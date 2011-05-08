
use zeroconf;

insert into DNSTable(hostname,interface,ip) values
('user1@cbox', 'WiFi', '192.168.1.1'),
('user3@cbox', 'ethernet', '192.168.3.3'),
('user4@cbox', 'bluetooth', '192.168.2.4'),
('user3@cbox', 'bluetooth', '192.168.2.3'),
('user4@cbox', 'WiFi', '192.168.1.4'),
('user3@cbox', 'WiFi', '192.168.1.3'),
('user1@cbox', 'bluetooth', '192.168.2.1'),
('user1@cbox', 'ethernet', '192.168.3.1');

insert into services (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user1@cbox', 'printing', 'TCP', 'WiFi', 99, 'test1'),
('user3@cbox', 'Music', 'RUDP', 'ethernet', 99, 'test3'),
('user4@cbox', 'touchPad', 'TCP', 'bluetooth', 99, 'test4'),
('user3@cbox', 'RemoteDesktop', 'TCP', 'bluetooth', 99, 'test5'),
('user4@cbox', 'printing', 'RUDP', 'WiFi', 14, 'test6'),
('user3@cbox', 'printing', 'TCP', 'WiFi', 99, 'test7'),
('user1@cbox', 'RemoteDesktop', 'RUDP', 'bluetooth', 51, 'test8'),
('user1@cbox', 'touchPad', 'UDP', 'ethernet', 89, 'test9'),
('user3@cbox', 'touchPad', 'UDP', 'WiFi', 40, 'test10');


insert into myServices (hostname,serviceType,protocol,interface,port,TXTDATA) values
('user2@cbox', 'RemoteDesktop', 'TCP', 'WiFi', 99, 'mytest1'),
('user2@cbox', 'printing', 'TCP', 'WiFi', 89, 'mytest2'),
('user2@cbox', 'Music', 'UDP', 'ethernet', 99, 'mytest3');


insert into requestedServices (serviceType) values
('printing'),
('RemoteDesktop'),
('Music'),
('touchPad');




