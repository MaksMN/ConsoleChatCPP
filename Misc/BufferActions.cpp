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
    Misc::writeStringBuffer(data, cmd_buffer, Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount));
}

void BufferActions::writeDynDataPos(uint value, uint blockCount)
{
    Misc::writeIntBuffer(value, cmd_buffer, Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount));
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
    uint addr = Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount);
    if (addr == 0)
        return addr;
    return Misc::getInt(cmd_buffer, addr);
}

std::string BufferActions::getDynDataS(uint blockCount)
{
    uint addr = Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount);
    if (addr == 0)
        return "Запрошены данные за пределом буфера";

    return Misc::getString(cmd_buffer, addr);
}

uint BufferActions::getDynDataI(uint blockCount)
{
    uint addr = Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, blockCount);
    if (addr == 0)
        return addr;

    return Misc::getInt(cmd_buffer, addr);
}

ullong BufferActions::getPmUserID()
{
    return Misc::getInt(cmd_buffer, PM_USER_ID);
}

void BufferActions::setPmUserID(ullong id)
{
    Misc::writeIntBuffer(id, cmd_buffer, PM_USER_ID);
}

void BufferActions::PmUserIDNoFound()
{
    Misc::writeStringBuffer("NOT_USER", cmd_buffer, PM_USER_ID, false);
}

void BufferActions::clearPmUserID()
{
    setPmUserID(0);
}

bool BufferActions::isNotFoundPmUserID()
{
    return Misc::getString(cmd_buffer, 8, PM_USER_ID) == "NOT_USER";
}

void BufferActions::setDataPacketsCount(unsigned char value)
{
    cmd_buffer[DATA_PACKETS_ADDR] = value;
}
