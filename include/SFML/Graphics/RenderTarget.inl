

////////////////////////////////////////////////////////////
template <typename T>
void RenderTarget::setView(const T& view)
{
    if (&view != m_view.get())
    {
        m_view.reset(new T(view));
    }

    m_cache.viewChanged = true;

    // Update the modelview matrix for any lighting updates
    applyViewTransform();
}
