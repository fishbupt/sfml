// Copyright (c) 2019 Keysight Technologies. All rights reserved.
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Xsa.Presentation.Graph
{
    /// <summary>
    /// A TaskScheduler will always run task on a specified thread
    /// </summary>
    public sealed class SingleThreadTaskScheduler : TaskScheduler, IDisposable
    {
        #region Fields

        [ThreadStatic] private static bool _isExecuting;

        private readonly String _threadName;

        private Thread _thread;

        private readonly CancellationToken _cancellationToken;

        private readonly BlockingCollection<Task> _taskQueue;

        #endregion

        public SingleThreadTaskScheduler(CancellationToken cancellationToken, String name)
        {
            _cancellationToken = cancellationToken;
            _taskQueue = new BlockingCollection<Task>();
            _threadName = name;
        }

        public void Start()
        {
            _thread = new Thread(new ThreadStart(RunOnCurrentThread))
            {
                Name = _threadName
            };

            _thread.IsBackground = true;
            _thread.Start();
        }

        public Task Run(Action action)
        {
            return Task.Factory.StartNew(action,
                CancellationToken.None,
                TaskCreationOptions.None,
                this);
        }

        public void Complete() => _taskQueue.CompleteAdding();

        private void RunOnCurrentThread()
        {
            _isExecuting = true;

            try
            {
                foreach (var task in _taskQueue.GetConsumingEnumerable(_cancellationToken))
                {
                    TryExecuteTask(task);
                }
            }
            catch (OperationCanceledException)
            {
            }
            finally
            {
                _isExecuting = false;
            }
        }

        #region TaskScheduler Interface Implementation

        protected override IEnumerable<Task> GetScheduledTasks() => _taskQueue.ToArray();

        protected override void QueueTask(Task task)
        {
            try
            {
                _taskQueue.Add(task, _cancellationToken);
            }
            catch(OperationCanceledException)
            { }
        }

        protected override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued)
        {
            if (taskWasPreviouslyQueued) return false;

            return _isExecuting && TryExecuteTask(task);
        }

        #endregion

        #region IDisposable 
        private bool _isDisposed = false; // To detect redundant calls

        void Dispose(bool disposing)
        {
            if (!_isDisposed)
            {
                if (disposing)
                {
                    // dispose managed state (managed objects).
                    _taskQueue.CompleteAdding();
                    _taskQueue.Dispose();
                }

                //  free unmanaged resources (unmanaged objects) and override a finalizer below.

                _isDisposed = true;
            }
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            GC.SuppressFinalize(this);
        }
        #endregion

    }
}
