# Консольный чат
## 16.9. Домашнее задание
### ЗАДАЧА: В качестве итогового домашнего задания вам необходимо реализовать концепцию чата, который вы разрабатывали по итогам 1 блока курса, с использованием контейнеров стандартной библиотеки шаблонов.

- Функционал консольного чата:
    - регистрация пользователей - логин, пароль, имя;
    - вход в чат по логину/паролю;
    - отправка сообщений конкретному пользователю;
    - обмен сообщениями между всеми пользователями чата одновременно;
    - Дополнительный функционал:
      - система администрирования и модерации;
      - система жалоб на сообщения в публичном чате;
      - блокировка пользователей;
      - Редактирование профиля пользователя. Смена имени и пароля.
- Структура приложения состоит из двух основных компонентов: база данных(backend) и пользовательский интерфейс(frontend).
  - База данных (backend) состоит из классов и структур, осуществляющих хранение сообщений и данных пользователей. 
  - Пользовательский интерфейс (frontend) обрабатывает ввод пользователя и на основе введенных данных запрашивает в базе отображает необходимую информацию и выводит ее на экран.
- Чат имеет два основных раздела: общий чат и раздел администратора. Основные разделы ветвятся на подразделы: публичная переписка, личные сообщения, профиль пользователя и прочие вспомогательные меню.
- На страницах сообщений можно выводить все сообщения разом и только ограниченное количество. Реализован постраничный вывод и навигация по спискам. Выбор опции "навигация по чату" актуальна если количество сообщений превышает количество выводимое на страницу.
- Для операций над сообщениями или пользователями нужно вводить userID или messageID.
- Для использования чата надо выполнять инструкции на экране и вводить предлагаемые значения. Есть защита от неверного ввода.

**Типы классы и функции**:

**DB/IDBcore** В нем выполнена основная задача домашнего задания. Это базовый шаблонный класс который содержит в себе шаблонный контейнер с типом vector:
```cpp
std::vector<std::shared_ptr<T>>
```
Наследники могут хранить в нем любую структуру данных или любой тип. Например класс работы с пользователями может хранить в нем тип User. Класс обработки сообщений может хранить там тип Message. В целом можно придумать любой тип данных и хранить его в контейнере базового класса.

IDBcore содержит в себе методы извлечения элементов из базы данных в виде списков и по индексу через переопределенный operator[].

Методы получения списков поддерживают пагинацию. Это постраничный вывод сообщений. Позволяют не выводить все сразу, а только последние 10 сообщений или выбрать конкретное сообщение в базе.

**DB/Structs/User.h, DB/Structs/Message.h, DB/Structs/Complaint.h** - основные структуры данных. Элементы контейнера в базе данных.

**DB/DBusers.h, DB/DBmessages.h, DB/DBcomplaints.h,** наследуют IDBcore и предназначены для обеспечения взаимодействия с фронтендом. Через эти классы основной контейнер изолирован от фронта. Через его методы фронт получает и передает данные. Запрашивает списки сообщений, передает новые сообщения в базу.

**Chat** - обеспечивает работу фронтенда.
**Chat/ChatRun** - точка входа.
**Chat/IChatInterface** базовый класс который наследуют **Chat/Users/UserInterface** и **Chat/Admin/AdminInterface**
Все эти классы обеспечивают интерактивное взаимодействие с пользователем программы. В них реализовано множество методов которые путем ветвления вызывают и сообщают друг другу о действиях пользователя. Например publicChat вызывает метод sendMessage. После чего метод sendMessage возвращает в publicChat информацию о действиях пользователя на основе которых publicChat может вызвать другие методы.

**Вспомогательные классы и функции:**

**Misc/UserInput** - шаблонный класс. Реализует обработку ввода пользователя. Обрабатывает неверный ввод. Позволяет задать возвращаемые значения отличные от типа входных данных. Например если пользователю надо ввести строки "да" или "нет", в качестве возвращаемых значений можно задать элементы перечислителя chat::yes и chat::no, что очень сильно упрощает процессы ветвления в коде программы. В аргументах конструктора передаются строковые переменные которые надо вывести на экран перед запросом ввода. Методы класса обеспечивают множественный и сквозной ввод. Когда пользователь должен ввести конкретные значения или когда output = input. Методы IOcin, IOcinThrough обеспечивают множественный и сквозной ввод с использованием std::cin. Методы IOgetline, IOgetlineThrough обрабатывают ввод с использованием std::getline.

**Misc/Flags** - обеспечивает операции с битовыми флагами. 

**Misc/sha1** - хеширует пароли.

**Misc/DateTime** - преобразует timestamp в обычное время.