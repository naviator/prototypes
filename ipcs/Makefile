.PHONY: a_test b_test deploy

a_test:
	kubectl cp main.cpp a:/tmp/main.cpp -c a
	kubectl exec -t a -c a -- g++ /tmp/main.cpp -o /tmp/a.out
	kubectl exec -t a -c a -- /tmp/a.out generate

b_test:
	kubectl cp main.cpp a:/tmp/main.cpp -c b
	kubectl exec -t a -c b -- g++ /tmp/main.cpp -o /tmp/a.out
	kubectl exec -t a -c b -- /tmp/a.out process

deploy:
	kubectl apply -f ipcs.yml

delete:
	kubectl delete -f ipcs.yml
