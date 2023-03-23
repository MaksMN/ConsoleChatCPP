#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <exception>

/// @brief Создает станицу обработки ввода пользователя
/// @tparam I - Тип данных ввода
/// @tparam O - Тип данных вывода
template <typename I, typename O>
class UserInput
{
private:
    std::string Description;
    std::string MainMessage;
    std::string FailMessage;
    int ioCount = 0;
    int ioLength = 0;
    std::unique_ptr<I[]> inputs;
    std::unique_ptr<O[]> outputs;
    bool throughIO; // output = input
    bool matchedIO; // inputs[n] -> outputs[n]
    bool filterInput;
    void addInputs(){};
    void addOutputs(){};

public:
    /// @brief Создает полную страницу множественного пользовательского ввода
    /// @param description - Заголовок страницы
    /// @param mainMessage - Сообщение пользователю
    /// @param failMessage - Сообщение при неверном вводе
    /// @param ioCapacity - Количество возможных вариантов ввода
    UserInput(std::string description, std::string mainMessage, std::string failMessage, int ioCapacity = 1);

    /// @brief Создает страницу сквозного пользовательского ввода. output = input.
    /// @param description - Заголовок страницы
    /// @param mainMessage - Сообщение пользователю
    UserInput(std::string description, std::string mainMessage);
    UserInput() = default;
    ~UserInput() = default;

    /// @brief Задает возможные варианты множественного ввода. Количество аргументов должно быть равным ioCapacity конструктора
    /// @tparam IO
    /// @tparam ...Args
    /// @param value
    /// @param ...args
    /// @exception out_of_range
    template <typename IO, typename... Args>
    void addInputs(IO value, Args... args);

    /// @brief Задает возможные варианты множественного вывода в соответствии addInputs.
    /// @tparam IO
    /// @tparam ...Args
    /// @param value
    /// @param ...args
    /// @exception out_of_range
    template <typename IO, typename... Args>
    void addOutputs(IO value, Args... args);

    /// @brief Задает возможные варианты множественного ввода и вывода. output = input
    void addIO(){};
    template <typename IO, typename... Args>
    void addIO(IO value, Args... args);

    /// @brief Отображает страницу обработки множественного ввода пользователя
    /// @return
    O IOAction();

    /// @brief Отображает страницу обработки сквозного ввода пользователя output = input
    /// @return
    I throughIOAction();

    void setDescription(std::string newText);
    void setMainMessage(std::string newText);
    void setFailMessage(std::string newText);
};

template <typename I, typename O>
inline UserInput<I, O>::UserInput(std::string description, std::string mainMessage, std::string failMessage, int ioCapacity)
{
    Description = description;
    MainMessage = mainMessage;
    FailMessage = failMessage;
    ioLength = ioCapacity;
    inputs = std::make_unique<I[]>(ioLength);
    outputs = std::make_unique<O[]>(ioLength);
    throughIO = false;
}

template <typename I, typename O>
inline UserInput<I, O>::UserInput(std::string description, std::string mainMessage)
{
    Description = description;
    MainMessage = mainMessage;
    throughIO = true;
    inputs = std::make_unique<I[]>(1);
    outputs = std::make_unique<O[]>(1);
}

template <typename I, typename O>
inline O UserInput<I, O>::IOAction()
{
    if (throughIO)
    {
        throw "Multiple input is disabled. Use throughIOAction() method.";
    }
    // Множественный ввод
    I userInput;
    do
    {
        if (!Description.empty())
        {
            std::cout << Description << std::endl;
        }
        std::cout << MainMessage;
        std::cin >> userInput;
        std::cin.clear();
        while (std::cin.get() != '\n')
        {
        }

        for (int i{0}; i < ioLength; i++)
        {
            if (userInput == inputs[i])
            {
                return outputs[i];
            }
        }
        std::cout << FailMessage << std::endl;
    } while (1);
}

template <typename I, typename O>
inline I UserInput<I, O>::throughIOAction()
{
    // Сквозной ввод
    I throughInput;
    std::cout << MainMessage;
    std::cin >> throughInput;
    std::cin.clear();
    while (std::cin.get() != '\n')
    {
    }
    return throughInput;
}

template <typename I, typename O>
inline void UserInput<I, O>::setDescription(std::string newText)
{
    Description = newText;
}

template <typename I, typename O>
inline void UserInput<I, O>::setMainMessage(std::string newText)
{
    MainMessage = newText;
}

template <typename I, typename O>
inline void UserInput<I, O>::setFailMessage(std::string newText)
{
    FailMessage = newText;
}

template <typename I, typename O>
template <typename IO, typename... Args>
inline void UserInput<I, O>::addInputs(IO value, Args... args)
{
    if (ioCount >= ioLength)
        throw std::out_of_range("Out of range array: inputs");
    inputs[ioCount++] = value;
    addInputs(args...);
    ioCount = 0;
}

template <typename I, typename O>
template <typename IO, typename... Args>
inline void UserInput<I, O>::addOutputs(IO value, Args... args)
{
    if (ioCount >= ioLength)
        throw std::out_of_range("Out of range array: outputs");
    outputs[ioCount++] = value;
    addOutputs(args...);
    ioCount = 0;
}

template <typename I, typename O>
template <typename IO, typename... Args>
inline void UserInput<I, O>::addIO(IO value, Args... args)
{
    if (ioCount >= ioLength)
        throw std::out_of_range("Out of range arrays: inputs or outputs");
    inputs[ioCount] = value;
    outputs[ioCount++] = value;
    addIO(args...);
    ioCount = 0;
}