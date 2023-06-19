#include "BufferActions.h"

BufferActions::BufferActions(char (&_cmd_buffer)[CMD_BUFFER]) : cmd_buffer(_cmd_buffer) {}

void BufferActions::removeFlag(sv::options option)
{
    cmd_buffer[FLAGS_ADDR] = (char)f.removeFlag((sv::options)cmd_buffer[FLAGS_ADDR], option);
}

bool BufferActions::hasFlag(sv::options option)
{
    return f.hasFlag(cmd_buffer[FLAGS_ADDR], option);
}

void BufferActions::writeDynDataPos(std::string data, uint blockCount)
{
    if (blockCount == 0)
    {
        Misc::writeStringBuffer(data, cmd_buffer, DYN_DATA_ADDR);
    }
    else
    {
        Misc::writeStringBuffer(data, cmd_buffer, Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount));
    }
}

void BufferActions::writeDynDataPos(uint value, uint blockCount)
{
    if (blockCount == 0)
    {
        Misc::writeIntBuffer(value, cmd_buffer, DYN_DATA_ADDR);
    }
    else
    {
        Misc::writeIntBuffer(value, cmd_buffer, Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount));
    }
}

char BufferActions::getDynDataAddr()
{
    return cmd_buffer[DYN_DATA_PTR_ADDR];
}

ullong BufferActions::getSessionKey()
{
    return Misc::getLong(cmd_buffer, SESSION_KEY_ADDR);
}

void BufferActions::setSessionKey(ullong key)
{
    Misc::writeUlongBuffer(key, cmd_buffer, SESSION_KEY_ADDR);
}

sv::pagination BufferActions::getPaginationMode()
{
    return (sv::pagination)cmd_buffer[PG_MODE_ADDR];
}

void BufferActions::setPaginationMode(sv::pagination mode)
{
    cmd_buffer[PG_MODE_ADDR] = mode;
}

uint BufferActions::getPgStart()
{
    return Misc::getInt(cmd_buffer, PG_START_ADDR);
}

void BufferActions::setPgStart(uint value)
{
    Misc::writeIntBuffer(value, cmd_buffer, PG_START_ADDR);
}

uint BufferActions::getPgPerPage()
{
    return Misc::getInt(cmd_buffer, PG_PERPAGE_ADDR);
}

void BufferActions::setPgPerPage(uint value)
{
    Misc::writeIntBuffer(value, cmd_buffer, PG_PERPAGE_ADDR);
}

uint BufferActions::getPgEnd()
{
    return Misc::getInt(cmd_buffer, PG_END_ADDR);
}

void BufferActions::setPgEnd(uint value)
{
    Misc::writeIntBuffer(value, cmd_buffer, PG_END_ADDR);
}

uint BufferActions::getDynDataSize(uint blockCount)
{
    return Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount);
}

std::string BufferActions::getDynData(uint blockCount)
{
    uint addr = Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount);
    if (addr == 0)
        return "Запрошены данные за пределом буфера";

    return Misc::getString(cmd_buffer, addr);
}
