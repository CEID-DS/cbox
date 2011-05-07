drop DATABASE if exists zeroconf;
create DATABASE zeroconf;

use zeroconf;

create table DNSTable
(
	hostname char(20) not null,
	interface char(10) not null,
	ip char(40) not null,
	primary key(hostname, interface)
)engine=innoDB default character set utf8 collate utf8_general_ci;

create table services
(
	hostname char(20) not null,  /*foreign key of DNSTable ?*/
	serviceType char(15) not null,
	protocol char(5) not null,
	interface char(10),
	port int(5),
	TXTDATA char(20),
	advertised bool,
	questioned bool,
	primary key(hostname,serviceType),
	foreign key(hostname) references DNSTable(hostname)
	on delete cascade on update cascade
)engine=innoDB default character set utf8 collate utf8_general_ci;

create table myServices
(
	hostname char(20) not null,
	serviceType char(15) not null,
	protocol char(5) not null,
	interface char(10),
	port int(5),
	TXTDATA char(20),
	advertised bool,
	questioned bool,
	primary key(hostname,serviceType)
)engine=innoDB default character set utf8 collate utf8_general_ci;


create table requestedServices
(
	reqSrvId int auto_increment,
	serviceType char(15),
	unique(serviceType),
	primary key(reqSrvId)
)engine=innoDB default character set utf8 collate utf8_general_ci auto_increment=1;

grant select, insert, update, delete on zeroconf.* to zeroUser identified by 'password';
flush privileges;
