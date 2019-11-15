
        /// <summary>
        /// This Method do library level initialization
        /// SFML use a shared OpenGL context to create other OpenGL context after the first context was created
        /// call this method to create a shared OpenGL context
        /// </summary>
        static void Initialize()
        {
            sf::Context ctx;
        }
