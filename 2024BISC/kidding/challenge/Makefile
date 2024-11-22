all:
	docker cp main.c kidding:/root/main.c
	docker exec -it kidding gcc /root/main.c -o /tmp/vuln
	docker cp kidding:/tmp/vuln .