#include "ResourceLoader.h"

namespace v3d
{
namespace stream
{

template <class T>
TResourceLoader<T>::TResourceLoader()
{
}

template <class T>
TResourceLoader<T>::~TResourceLoader()
{
    m_pathes.clear();
    m_decoders.clear();

    unloadAll();
}

template <class T>
T TResourceLoader<T>::get(const std::string& name)
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
    TResourceLoader::unload(name, std::integral_constant<bool, std::is_pointer<T>::value>::type());
}

template <class T>
void TResourceLoader<T>::unload(T resource)
{
    TResourceLoader::unload(resource, std::integral_constant<bool, std::is_pointer<T>::value>::type());
}

template<class T>
inline void TResourceLoader<T>::unloadAll()
{
    TResourceLoader::unloadAll(std::integral_constant<bool, std::is_pointer<T>::value>::type());
}

template <class T>
void TResourceLoader<T>::registerPath(const std::string& path)
{
    if (!path.empty())
    {
        m_pathes.push_back(path);
    }
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

template<class T>
void TResourceLoader<T>::unload(const std::string& name, std::true_type)
{
    auto it = m_resources.find(name);
    if (it != m_resources.end())
    {
        delete it->second;
        it->second = nullptr;

        m_resources.erase(it);
    }
}

template<class T>
void TResourceLoader<T>::unload(const std::string& name, std::false_type)
{
    auto it = m_resources.find(name);
    if (it != m_resources.end())
    {
        it->second = nullptr;

        m_resources.erase(it);
    }
}

template<class T>
void TResourceLoader<T>::unload(T resource, std::true_type)
{
    auto predDelete = [resource](const std::pair<std::string, T>& pair) -> bool
    {
        return pair.second == texture;
    };

    auto it = std::find_if(m_resources.begin(), m_resources.end(), predDelete);
    if (it != m_resources.end())
    {
        delete it->second;
        it->second = nullptr;

        m_resources.erase(it);
    }
}

template<class T>
void TResourceLoader<T>::unload(T resource, std::false_type)
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

template<class T>
void TResourceLoader<T>::unloadAll(std::true_type)
{
    for (auto& it : m_resources)
    {
        delete it.second;
        it.second = nullptr;
    }
    m_resources.clear();
}

template<class T>
void TResourceLoader<T>::unloadAll(std::false_type)
{
    m_resources.clear();
}

template <class T>
void TResourceLoader<T>::insert(T resource, const std::string& key)
{
    m_resources.insert(std::map<const std::string, T>::value_type(key, resource));
}

template <class T>
const decoders::DecoderPtr TResourceLoader<T>::findDecoder(const std::string& extension)
{
    auto predCanDecode = [extension](const decoders::DecoderPtr& decoder) -> bool
    {
        return decoder->isExtensionSupported(extension);
    };

    std::vector<decoders::DecoderPtr>::iterator iter = std::find_if(m_decoders.begin(), m_decoders.end(), predCanDecode);
    if (iter == m_decoders.end())
    {
        return nullptr;
    }

    return (*iter);
}

} //namespace stream
} //namespace v3d
