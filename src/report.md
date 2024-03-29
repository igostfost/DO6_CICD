# Basic CI/CD

### Part 1. Настройка **gitlab-runner**

>**== Задание ==**
>
>##### Поднять виртуальную машину *Ubuntu Server 20.04 LTS*
>*Будьте готовы, что в конце проекта нужно будет сохранить дамп образа виртуальной машины*

- ![basic_ci_cd](./images/1_1.png)
>##### Скачать и установить на виртуальную машину **gitlab-runner**
- скачали:
- ![basic_ci_cd](./images/1_2.png)
- установили:
- ![basic_ci_cd](./images/1_3.png)
>##### Запустить **gitlab-runner** и зарегистрировать его для использования в текущем проекте (*DO6_CICD*)
>- Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе.
- ![basic_ci_cd](./images/1_4.png)

### Part 2. Сборка

>**== Задание ==**
>
>#### Написать этап для **CI** по сборке приложений из проекта *C2_SimpleBashUtils*:
>
>##### В файле _gitlab-ci.yml_ добавить этап запуска сборки через мейк файл из проекта _C2_
>
>##### Файлы, полученные после сборки (артефакты), сохранять в произвольную директорию со сроком хранения 30 дней.
- Скопировал папки cat и grep из проекта simpleBashUtils в папку src проекта CICD: 
- ![basic_ci_cd](./images/2_1.png)
- создал и описал файл .gitlab-ci.yml
    - Файл .gitlab-ci.yml - это файл конфигурации для настройки и определения задач и пайплайнов непрерывной интеграции и непрерывной доставки (CI/CD) в GitLab. В этом файле вы описываете, какие шаги должны выполняться автоматически при каждом пуше кода в ваш репозиторий. После добавления .gitlab-ci.yml файла в ваш репозиторий, GitLab CI/CD будет автоматически создавать пайплайны и выполнять задачи согласно вашим настройкам. Вы сможете видеть результаты выполнения задач в веб-интерфейсе GitLab, а также настраивать уведомления, автоматическое развертывание и другие аспекты CI/CD процесса.
- ![basic_ci_cd](./images/2_2.png)

### Part 3. Тест кодстайла

>**== Задание ==**
>
>#### Написать этап для **CI**, который запускает скрипт кодстайла (*clang-format*):
>
>##### Если кодстайл не прошел, то "зафейлить" пайплайн
- ![basic_ci_cd](./images/3_1.png)
>##### В пайплайне отобразить вывод утилиты *clang-format*
- ![basic_ci_cd](./images/3_2.png)

### Part 4. Интеграционные тесты

>**== Задание ==**
>
>#### Написать этап для **CI**, который запускает ваши интеграционные тесты из того же проекта:
>
>##### Запускать этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно
>
>##### Если тесты не прошли, то "зафейлить" пайплайн
- Мои тесты возвращают 2 строки:  количество SUCCESS и FAIL тестов. Таким образом для фела пеплайна необходимо чтобы в строке FAIL было значение не 0, то есть хотябы 1 тест был зафейлен. Для этого в скрипте запускаем тесты и смотрим на строку FAIL, если она не равна 0, то роняем пеплайн.
- ![basic_ci_cd](./images/4_1.png)
>##### В пайплайне отобразить вывод, что интеграционные тесты успешно прошли / провалились
- ![basic_ci_cd](./images/4_2.png)

### Part 5. Этап деплоя

>**== Задание ==**
>
>##### Поднять вторую виртуальную машину *Ubuntu Server 20.04 LTS*
- ![basic_ci_cd](./images/5_1.png)
>#### Написать этап для **CD**, который "разворачивает" проект на другой виртуальной машине:
>##### Запускать этот этап вручную при условии, что все предыдущие этапы прошли успешно
- необходимо перенастроить сетевые адаптеры обеих машин следующим образом:
- ![basic_ci_cd](./images/5_4.png)
- разворачивание проекта на другой машине будет осуществлятся через ssh туннель.Для этого нужно установить ssh-server 
- ключевая сложность этого этапа это права доступа. Каждый раз запуская скрипт нам требуется доступ к папкам на удаленном сервере. Все работы выполняет gitlab-runner через своего одноименного пользователя. Необходимо на сервере где запущен gitlab-runner сменить пользователя на gitlab-runner командой `su - gitlab-runner`, далее сгенерировать  ssh `ssh-keygen` ключ. Полученный ключ необходимо внести в файл /.ssh/authorized_keys на удаленном сервере командой `ssh-copy-id user@host` и если возникает ошибка доступа при переноса файлов, нужно делать пользователя, под которым заходишь на вторую машину владельцем директории в которую переносишь файлы `sudo chown -R $(whoami) `.
- добавили на вторую машину ssh ключ`
- ![basic_ci_cd](./images/5_7.png)
>##### Написать bash-скрипт, который при помощи **ssh** и **scp** копирует файлы, полученные после сборки (артефакты), в директорию */usr/local/bin* второй виртуальной машины
- ip машины получаем командой `ip a`
- ![basic_ci_cd](./images/5_5.png)
>##### В файле _gitlab-ci.yml_ добавить этап запуска написанного скрипта
- ![basic_ci_cd](./images/5_2.png)
>##### В случае ошибки "зафейлить" пайплайн
- ![basic_ci_cd](./images/5_3.png)
>В результате вы должны получить готовые к работе приложения из проекта *C2_SimpleBashUtils* (s21_cat и s21_grep) на второй виртуальной машине.
- ![basic_ci_cd](./images/5_6.png)

### Part 6. Дополнительно. Уведомления

**== Задание ==**

##### Настроить уведомления о успешном/неуспешном выполнении пайплайна через бота с именем "[ваш nickname] DO6 CI/CD" в *Telegram*
- первым делом необходимо получить токен бота. Для этого используем инфраструктуру телеграма, а точнее главного бота "BotFather". Выполняем команды указаные ниже и получаем токен.
- ![basic_ci_cd](./images/6_1.png)
- ![basic_ci_cd](./images/6_2.png)
- далее необходимо получить id чата. Для этого в адресной строке браузера забиваем `https://api.telegram.org/bot<YOUR_BOT_TOKEN>/getUpdates`, уже находясь на указанной странице отправьте любое сообщение в чат бота. На странице отобразятся данные, включая id чата в который вы отправили сообщение (поле chat->id):
- ![basic_ci_cd](./images/6_3.png)
- далее пишем скрипт который будет контролировать работу каждого этама CI/CD:
- ![basic_ci_cd](./images/6_5.png)
- добавляем выполнение скрипта после каждого этапа, итоговый файл .gitlab-ci.yml выглядит так:
- ![basic_ci_cd](./images/6_6.png)
- коммитим, пушим и проверяем работу бота
- ![basic_ci_cd](./images/6_4.png)
