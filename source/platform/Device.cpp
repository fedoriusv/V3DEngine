#include "Device.h"
#include "utils/Logger.h"

namespace v3d
{
namespace platform
{

Device::Device(renderer::RendererPtr renderer, bool isThreaded)
    : m_isThreaded(isThreaded)
    , m_isRunning(false)
    , m_semaphore(1)
    , m_render(renderer)
{
    if (Device::isThreaded())
    {
        bool result = m_thread.run(std::bind(&Device::threadWorker, this, std::placeholders::_1), this);
        if (!result)
        {
            ASSERT(false, "Device::Device: thread has not started");
        }
    }
}

Device::~Device()
{
    m_thread.wait(true);
}

bool Device::init()
{
    ASSERT(m_render, "Device::getRenderer() render is nullptr");

    if (Device::isThreaded())
    {
        m_commandBuffer.write((s32)eCmdInit);
        
        //Test
        m_commandBuffer.write(s32(5));
    }
    else
    {
        m_render->init();
    }

    return true;
}

const renderer::RendererPtr Device::getRenderer() const
{
    return m_render;
}

bool Device::isThreaded() const
{
    return m_isThreaded;
}

void Device::threadWorker(void* data)
{
    stream::MemoryStream& stream = reinterpret_cast<Device*>(data)->m_commandBuffer;
    m_isRunning = true;

    while (m_isRunning)
    {
        if (!m_thread.isRunning())
        {
            m_isRunning = false;
        }

        Device::wait();
        Device::runCommand(stream);
    }
}

void Device::runCommand(const stream::MemoryStream& stream)
{
    Device::notify();

    s32 cmd;
    stream.read(cmd);
    EDeviceCommand command = (EDeviceCommand)cmd;

    switch (command)
    {
        case eCmdInit:
        {
            u32 test;
            stream.read(test);
            LOG_DEBUG("Device::runCommand test: %d", test);
        }

        case eCmdTerminate:
        {
            m_isRunning = false;
            Device::wait(true);
        }

        default:
            ASSERT(false, "Unknown command");
    }
    

    

}

void Device::wait(bool result)
{
    if (result)
    {
        m_semaphore.tryWait();
    }
    else
    {
        m_semaphore.wait();
    }
}

void Device::notify()
{
    m_semaphore.post();
}

} //namespace platform
} //namespace v3d
