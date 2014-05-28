#include "FileStream.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>

using namespace f3d;
using namespace f3d::stream;
using namespace f3d::core;

FileStream::FileStream()
    : m_fileHandler(nullptr)
    , m_isOpen(false)
{
}

FileStream::FileStream(const std::string& file, EOpenMode openMode)
{
    open(file, openMode);
}


FileStream::~FileStream()
{
    close();
}

bool FileStream::open(const std::string& file, EOpenMode openMode)
{
    const char* mode = 0;

    if ((openMode & e_in) == openMode)
    {
        mode = "rb";
    }
    else if ((openMode & e_out) == openMode)
    {
        mode = "wb";
    }
    else if ((openMode & e_app) == openMode)
    {
        mode = "ab";
    }
    else if ((openMode & (e_in | e_out)) == openMode)
    {
        mode = "r+b";
    }
    else if ((openMode & (e_out | e_app)) == openMode)
    {
        mode = "ab";
    }
    else if (openMode & e_trunc)
    {
        mode = "wb+";
    }
    else if (openMode & e_create)
    {
        mode = "wb+";
    }
    else
    {
        mode = "rb";
    }

    m_fileName = file;

    m_fileHandler = fopen(file.c_str(), mode);

    m_isOpen = m_fileHandler != nullptr;

    return m_isOpen;
}

void FileStream::close()
{
    if (m_fileHandler)
    {
        fclose(m_fileHandler);
        m_fileHandler = nullptr;
    }
    m_fileSize = 0;
    m_fileName = "";
    m_isOpen = false;
}


bool FileStream::isOpen() const
{
    return m_isOpen;
}

u32 FileStream::read(void* buffer, const u32 size, const u32 count)
{
    const u32 ret = fread(buffer, size, count, m_fileHandler);
    return ret;
}

u32 FileStream::read(u8& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s8& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u16& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s16& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u32& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s32& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(u64& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(s64& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f32& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f64& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(f80& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(bool& value)
{
    const u32 ret = read(&value, sizeof(value), 1);
    return ret;
}

u32 FileStream::read(std::string& value)
{
    u32 strLen = 0;
    u32 ret = fread(&strLen, sizeof(u32), 1, m_fileHandler);
    c8* str = new c8[strLen];
    ret += read(&str, sizeof(c8), strLen);
    value = str;
    delete[] str;
    return ret;
}

u32 FileStream::write(void* buffer, const u32 size, const u32 count)
{
    const u32 ret = fwrite(buffer, size, count, m_fileHandler);
    return ret;
}

u32 FileStream::write(const u8 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const s8 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const u16 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const s16 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const u32 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const s32 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const u64 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const s64 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const f32 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const f64 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const f80 value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const bool value)
{
    const u32 ret = fwrite(&value, sizeof(value), 1, m_fileHandler);
    return ret;
}

u32 FileStream::write(const std::string& value)
{
    const u32 strLen = value.length();
    u32 ret = fwrite(&strLen, sizeof(u32), 1, m_fileHandler);
    ret += fwrite(value.c_str(), sizeof(c8), strLen, m_fileHandler);
    return ret;
}


void FileStream::seekBeg(const u32 offset)
{
    fseek(m_fileHandler, offset, SEEK_SET);
}

void FileStream::seekCur(const u32 offset)
{
    fseek(m_fileHandler, offset, SEEK_CUR);
}

u32 FileStream::tell()
{
    u32 tell = ftell(m_fileHandler);
    return tell;
}

u32 FileStream::size()
{
    if (m_fileSize == 0)
    {
        u32 currentPos = tell();
        fseek(m_fileHandler, 0, SEEK_END);
        m_fileSize = tell();
        fseek(m_fileHandler, currentPos, SEEK_SET);
    }
    return m_fileSize;
}

void* FileStream::map(const u32 size)
{
    return nullptr;
}

void FileStream::unmap()
{

}
std::string FileStream::getName() const
{
    return m_fileName;
}


bool FileStream::isFileExist(const std::string& fileName)
{
    return false;
}