
use zeroconf;

insert into services (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user1@cbox', 'service1', 'protocol1', 'interface1', 99, 'test1'),
('user3@cbox', 'service3', 'protocol3', 'interface3', 99, 'test3'),
('user4@cbox', 'service4', 'protocol1', 'interface2', 99, 'test4'),
('user3@cbox', 'service2', 'protocol1', 'interface2', 99, 'test5'),
('user4@cbox', 'service1', 'protocol3', 'interface1', 14, 'test6'),
('user3@cbox', 'service1', 'protocol1', 'interface1', 99, 'test7'),
('user1@cbox', 'service2', 'protocol3', 'interface2', 51, 'test8'),
('user1@cbox', 'service4', 'protocol2', 'interface3', 89, 'test9'),
('user3@cbox', 'service4', 'protocol2', 'interface1', 40, 'test10');


insert into services (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user2@cbox', 'service2', 'protocol1', 'interface1', 99, 'mytest1');


insert into requestedServices (serviceType) values
('service1'),
('service2'),
('service3'),
('service4');

insert into DNSTable(hostname,interface,ip) values
('user1@cbox', 'interface1', '192.168.1.1'),
('user3@cbox', 'interface3', '192.168.3.3'),
('user4@cbox', 'interface2', '192.168.2.4'),
('user3@cbox', 'interface2', '192.168.2.3'),
('user4@cbox', 'interface1', '192.168.1.4'),
('user3@cbox', 'interface1', '192.168.1.3'),
('user1@cbox', 'interface2', '192.168.2.1'),
('user1@cbox', 'interface3', '192.168.3.1');
