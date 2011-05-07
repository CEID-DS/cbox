
use zeroconf;

insert into DNSTable(hostname,interface,ip) values
('user1@cbox', 'interface1', '192.168.1.1'),
('user3@cbox', 'interface3', '192.168.3.3'),
('user4@cbox', 'interface2', '192.168.2.4'),
('user3@cbox', 'interface2', '192.168.2.3'),
('user4@cbox', 'interface1', '192.168.1.4'),
('user3@cbox', 'interface1', '192.168.1.3'),
('user1@cbox', 'interface2', '192.168.2.1'),
('user1@cbox', 'interface3', '192.168.3.1');

insert into services (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user1@cbox', 'service1', 'pr1', 'interface1', 99, 'test1'),
('user3@cbox', 'service3', 'pr3', 'interface3', 99, 'test3'),
('user4@cbox', 'service4', 'pr1', 'interface2', 99, 'test4'),
('user3@cbox', 'service2', 'pr1', 'interface2', 99, 'test5'),
('user4@cbox', 'service1', 'pr3', 'interface1', 14, 'test6'),
('user3@cbox', 'service1', 'pr1', 'interface1', 99, 'test7'),
('user1@cbox', 'service2', 'pr3', 'interface2', 51, 'test8'),
('user1@cbox', 'service4', 'pr2', 'interface3', 89, 'test9'),
('user3@cbox', 'service4', 'pr2', 'interface1', 40, 'test10');


insert into myServices (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user2@cbox', 'service2', 'pr1', 'interface1', 99, 'mytest1'),
('user2@cbox', 'service1', 'pr1', 'interface1', 89, 'mytest2'),
('user2@cbox', 'service3', 'pr2', 'interface3', 99, 'mytest3');


insert into requestedServices (serviceType) values
('service1'),
('service2'),
('service3'),
('service4');




