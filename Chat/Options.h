#pragma once

/// @brief Опции применяемые для ветвления.
namespace chat
{
    enum Results
    {
        in_login_page,
        in_register_page,
        in_chat_area,
        in_admin_area,
        in_public_chat_page,
        in_private_chat_page,
        in_user_profile_page,
        in_chat_navigation,
        exit_,
        back_,
        user_banned,
        register_success,
        register_cancel,
        login_cancel,
        login_success,
        admin_login_fail,
        yes_,
        no_,
        send_message,
        user_input,
        navigation_,
        complaint_,
        complaint_sended,
        private_,
        public_,
        search_user_byId,
        conversation_,
        admin_users_manage,
        admin_messages_manage,
        admin_complaint_manage,
        admin_ban_user,
        admin_unban_user,
        user_to_admin,
        admin_to_user,
        user_not_found,
        complaint_delete_message_ban,
        complaint_delete_message_noban,
        complaint_complete,
        complaint_delete
    };
    enum Pagination
    {
        last_page,
        element_,
        msg_per_page,
        select_message,
        select_last_page,
        in_chat
    };
};
