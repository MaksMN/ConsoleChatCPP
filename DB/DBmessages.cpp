#include "DBmessages.h"

std::shared_ptr<Message> DBmessages::addMessage(
    const uint &author_id,
    const uint &recipient_id,
    const std::wstring &text,
    msg::status status)
{
    _DB.push_back(std::make_shared<Message>(lastMsgID++, author_id, recipient_id, text, status));
    return _DB.back();
}

std::shared_ptr<Message> DBmessages::getMessageByID(uint id)
{
    return operator[](id);
}

std::vector<std::shared_ptr<Message>> DBmessages::getPrivateMsgList(uint &&author_id, uint &&recipient_id, uint &start, const uint &perPage, uint &end, bool last)
{
    if (_DB.empty())
    {
        return std::vector<std::shared_ptr<Message>>();
    }
    std::vector<std::shared_ptr<Message>> in;
    std::vector<std::shared_ptr<Message>> out;

    std::for_each(_DB.begin(), _DB.end(),
                  [&author_id, &recipient_id, &in](const auto &m)
                  {
                      bool cond = ((m->getAuthorID() == author_id) && (m->getRecipientID() == recipient_id)) ||
                                  ((m->getAuthorID() == recipient_id) && (m->getRecipientID() == author_id));
                      if (cond)
                          in.push_back(m);
                  });
    if (in.empty())
    {
        return std::vector<std::shared_ptr<Message>>();
    }

    if (last)
    {
        end = in.size();
        if (perPage >= in.size())
        {
            start = 0;
        }
        else
        {
            start = in.size() - perPage;
        }
    }

    end = start + perPage;
    if (start > in.size())
        start = 0;
    if (end > in.size())
        end = in.size();

    for (uint i{start}; i < end; i++)
    {
        if (i == in.size())
            break;
        out.push_back(in[i]);
    }
    return out;
}

void DBmessages::deleteMessage(uint id)
{
    auto it = std::find_if(_DB.begin(), _DB.end(),
                           [&id](const auto &m)
                           {
                               return m->getID() == id;
                           });

    if (_DB.end() != it)
        _DB.erase(it);
}
