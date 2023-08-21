# S21_SimpleDocker

## Part1. Ready-made docker

* Возьмемо официальный docker-образ с nginx и выкачаем его при помощи команды `docker pull` 
![pull_nginx](img/pull_nginx.png)

* Далее удостоверимся в наличии образа через команду `docker images` 
![docker_images](img/docker_images.png)

* Наконец, запустим docker-образ через команду `docker run -d [image_id|repository]` 
![docker_run_nginx](img/docker_run_nginx.png)
```
-d: это флаг, который указывает Docker на запуск контейнера в фоновом режиме (detached mode). Это означает, что контейнер будет работать в фоновом режиме, и командная строка будет освобождена для дальнейшего использования.
```

* Удостоверимся, что контейнер успешно запустился через команду `docker ps` 
![docker_ps_1](img/docker_ps_1.png)

* Теперь посмотрим информацию о контейнере через команду `docker inspect [container_id|container_name]` 
![docker_inspect](img/docker_inspect.png)

* Выведем размер контейнера  
![grep_size](img/grep_size.png)

* А теперь - список замапленных портов  
![mapped_ports](img/mapped_ports.png)

* И, наконец, IP контейнера  
![docker_ipaddress](img/docker_ipaddress.png)

* Остановим docker-образ командой `docker stop [container_id|container_name]` и проверим, что образ успешно остановился через уже знакомую команду `docker ps` 
![docker_stop](img/docker_stop.png)

* Теперь запустим docker-образ с портами 80:80 и 443:443 чере команду `docker run` 
![docker_run_8080](img/docker_run_8080.png)

* Удостоверимся, что все работает, открыв в браузере страницу по адресу `localhost` 
![docker_localhost](img/docker_localhost.png)

* Наконец, перезапустим контейнер через команду `docker restart [container_id|container_name]` и проверим, что контейнер снова запустился командой `docker ps` 
![docker_restart](img/docker_restart.png)

# Part2. Operations with container

* Для начала прочтем конфигурационный файл `nginx.conf` внутри docker-контейнера через команду `docker exec` 
![docker_restart](img/docker2_exec_cat.png)

* Теперь создадим локальный файл `nginx.conf` при помощи команды `touch nginx.conf` и настроем в нем выдачу страницы-статуса сервера по пути `/status` 
![docker2_myconf](img/docker2_myconf.png)

* Наконец, перенесем созданный файл внутрь docker-образа командой `docker cp` 
![docker2_cp](img/docker2_cp.png)

* И перезапустим nginx внутри docker-образа командой `docker exec [container_id|container_name] nginx -s reload` 
![docker2_exec_nginx](img/docker2_exec_nginx.png)

* Убедимся, что все работает, проверив страницу по адресу `localhost/status` 
![docker2_new_conf](img/docker2_new_conf.png)

* Теперь экспортируем наш контейнер в файл `container.tar` командой `docker export` 
![docker2_tar](img/docker2_tar.png)

* Затем удалим образ командой `docker rmi -f [image_id|repository]`, не удаляя перед этим контейнеры 
![docker2_remove_nginx](img/docker2_remove_nginx.png)

* После чего удалим остановленный контейнер командой `docker rm [container_id|container_name]` 
![docker2_remove_container](img/docker2_remove_container.png)

* Теперь импортируем контейнер обратно командой `docker import` и запустим импортированный контейнер уже знакомой командой `dicker run` 
![docker2_import](img/docker2_import.png)

* Наконец проверим, что по адресу `localhost/status` выдается страничка со статусом сервера nginx 
![docker2_localhost8080](img/docker2_localhost8080.png)

## Part3. Mini web server

* Чтобы создать свой мини веб-сервер, необходимо создать .c файл, в котором будет описана логика сервера (в нашем случае - вывод сообщения `Hello World!`), а также конфиг `nginx.conf`, который будет проксировать все запросы с порта 81 на порт 127.0.0.1:8080 
![docker3_server](img/docker3_server.png) 
![docker3_conf](img/docker3_conf.png)

* Теперь выкачаем новый docker-образ и на его основе запустим новый контейнер 
![docker3_new_nginx](img/docker3_new_nginx.png)

* После перекинем конфиг и логику сервера в новый контейнер 
![docker3_copied](img/docker3_copied.png)

* Затем установим требуемые утилиты для запуска мини веб-сервера на FastCGI, в частности `spawn-fcgi` и `libfcgi-dev` 
![docker3_install](img/docker3_install.png)

* Наконец скомпилируем и запустим наш мини веб-сервер через команду `spawn-fcgi` на порту 8080 
![docker3_start_server](img/docker3_start_server.png)

* Чтобы удостовериться, что все работает корректно, проверим, что в браузере по адресу `localhost:81` отдается написанная нами страница 
![docker3_hello_world](img/docker3_hello_world.png)

```
!!Примечание!!

Если во время развертывания сервера в контейнере у вас возникнут непредвиденные ошибки, например, неправильно написанный конфиг или что-то вроде, из-за чего сервер будет работать некорректно, вам придется убить старый процесс при помощи команды [kill] по его PID, узнать который можно благодаря утилите [sockstat] командой [sockstat -lu], что показано на приведенном ниже скриншоте. После описанных манипуляций вы сможете вновь развернуть сервер на необходимый порт
```
![docker3_kill](img/docker3_kill.png)

## Part4. Your own docker

* Напишем свой docker-образ, который собирает исходники 3-й части, запускает на порту `80`, после копирует внутрь написанный нами `nginx.conf` и, наконец, запускает `nginx` (ниже приведены файлы `run.sh` и `Dockerfile`, файлы `nginx.conf` и `server.c` остаются с 3-й части)

![docker4_runsh](img/docker4_runsh.png)  
![docker4_dockerfile](img/docker4_dockerfile.png)  

* Соберем написанный docker-образ через команду `docker build`, при этом указав имя и тэг нашего контейнера  
![docker4_build.png](img/docker4_build.png)  

* Теперь удостоверимся, что все собралось, проверив наличие соответствующего образа командой `docker images`  
![docker4_images.png](img/docker4_images.png)  

* После запустим собранный docker-образ с мапингом порта `81` на порт `80` локальной машины, а также мапингом папки `./nginx` внутрь контейнера по адресу конфигурационных файлов nginx'а, и проверим, что страничка написанного сервера по адресу 
![docker4_run_server.png](img/docker4_run_server.png)

```
!!Примечание!!
Если при проверке адреса localhost вы увидете ошибку 502, остановите запущенный docker-образ, после исправьте ошибки в конфигурационных файлах и заново запустите собранный docker-образ
```

* Теперь добавим в файл `nginx.conf` проксирование странички `/status`, по которой необходимо отдавать статус сервера `nginx  
![docker4_nginx.png](img/docker4_nginx.png)

* Теперь перезапустим `nginx` в своем docker-образе командой `nginx -s reload`  
![docker4_reload_serv.png](img/docker4_reload_serv.png)

* Наконец, проверим, что по адресу `localhost/status` выдается страничка со тсатусом сервера `nginx`  
![docker4_success.png](img/docker4_success.png)


## Part5. Dockle

```
!!Примечание!!
Перед выполнением данного шага необходимо установить утилиту [dockle], инструкция по установке [https://github.com/goodwithtech/dockle], если машина не видит утилиту [https://github.com/aquasecurity/trivy/issues/2432], также рекомендую добавить своего пользователя в группу [docker]
```

* Просканируем docker-образ из предыдущего задания на предмет наличия ошибок командой `dockle [image_id|repository]`  
![part5_troubles.png](img/part5_troubles.png)

* Далее исправим конфигурационные файлы docker-образа так, чтобы при проверке через утилиту `dockle` не возникало ошибок и предупреждений (для Part5 я создал отдельный контейнер с тэгом `part5`, куда подгрузил измененные конфиги)  
![part5_new_build.png](img/part5_new_build.png)
![part5_check_build.png](img/part5_check_build.png)
![part5_success.png](img/part5_success.png)

## Part6. Basic Docker Compose

```
!!Примечание!!
Перед выполнением данного шага необходимо установить утилиту [docker-compose], инструкция по установке [https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-compose-on-ubuntu-20-04]
```

* Для начала остановим все запущенные контейнеры командой `docker stop`  
![docker6_remove.png](img/docker6_remove.png)

* Затем изменим конфигурационные файлы (их можно найти в папке `src/part6_compose`)

* Теперь сбилдим контейнер командой `docker-compose build`
![part6_build.png](img/part6_build.png)  

* После необходимо поднять сбилженный контейнер командой `docker compose up`
![part6_dockerup.png](img/part6_dockerup.png)  

* В завершение насладимся плодами своей усердной работы, удостоверившись, что по адресу `localhost` отдается страничка с надписью `Hello World!`
![part6_success.png](img/part6_success.png)  
