using namespace v3d;
using namespace stream;

template <class T>
TResourceLoader<T>::TResourceLoader()
{
}

template <class T>
TResourceLoader<T>::~TResourceLoader()
{
    TResourceLoader::unloadAll();

    m_pathes.clear();
    m_decoders.clear();
}

template <class T>
const T TResourceLoader<T>::get(const std::string& name)
{
    auto it = m_resources.find(name);
    if (it != m_resources.end())
    {
        return it->second;
    }

    return nullptr;
}

template <class T>
void TResourceLoader<T>::unload(const std::string& name)
{
    auto it = m_resources.find(name);
    if (it != m_resources.end())
    {
        m_resources.erase(it);
    }
}

template <class T>
void TResourceLoader<T>::unload(const T& resource)
{
    auto predDelete = [resource](const std::pair<std::string, T>& pair) -> bool
    {
        return pair.second == texture;
    };

    auto it = std::find_if(m_resources.begin(), m_resources.end(), predDelete);

    if (it != m_resources.end())
    {
        m_resources.erase(it);
    }
}

template <class T>
void TResourceLoader<T>::unloadAll()
{
    m_resources.clear();
}

template <class T>
void TResourceLoader<T>::registerPath(const std::string& path)
{
    m_pathes.push_back(path);
}

template <class T>
void TResourceLoader<T>::unregisterPath(const std::string& path)
{
    auto it = std::find(m_pathes.begin(), m_pathes.end(), path);
    if (it != m_pathes.end())
    {
        m_pathes.erase(std::remove(m_pathes.begin(), m_pathes.end(), *it), m_pathes.end());
    }
}

template <class T>
void TResourceLoader<T>::registerDecoder(decoders::DecoderPtr decoder)
{
    m_decoders.push_back(decoder);
}

template <class T>
void TResourceLoader<T>::unregisterDecoder(decoders::DecoderPtr& decoder)
{
    auto it = std::find(m_decoders.begin(), m_decoders.end(), decoder);
    if (it != m_decoders.end())
    {
        m_decoders.erase(std::remove(m_decoders.begin(), m_decoders.end(), *it), m_decoders.end());
    }
}

template <class T>
void TResourceLoader<T>::insert(const T& resource, const std::string& key)
{
    m_resources.insert(std::map<std::string, T>::value_type(key, resource));
}
