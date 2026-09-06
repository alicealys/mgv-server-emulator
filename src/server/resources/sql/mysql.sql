-- query:mgssd.users.create
create table if not exists `users`
(
	user_id					bigint unsigned	not null	auto_increment,
	account_id				bigint unsigned	not null	 unique,
	session_id				char(32)		default null unique,
	password_hash			varchar(32)		default null,
	crypto_key				char(32)		default null,
	currency				varchar(32)		default null,
	ex_ip					varchar(15)		default null,
	in_ip					varchar(15)		default null,
	ex_port					int unsigned	default 0,
	in_port					int unsigned	default 0,
	nat						int unsigned	default 0,
	current_player_id		bigint unsigned default null,
	last_update				datetime		default null,
	user_creation_date		datetime		default null,
	primary key (`user_id`)
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id				bigint unsigned	not null	auto_increment,
	f_user_id				bigint unsigned unique,
	player_index			bigint unsigned default 0,
	player_creation_date	datetime        not null,
	primary key (`player_id`),
	foreign key (`f_user_id`) references `users`(`user_id`)
)
-- query:mgssd.players.remove_insert_trigger
drop trigger if exists players_insert_trigger
-- query:mgssd.players.add_insert_trigger
create trigger players_insert_trigger
before insert on players
for each row
begin
	declare total_rows int;
    select count(*) into total_rows 
	from players where f_user_id = NEW.f_user_id;
	set NEW.player_index = total_rows;
end
-- query:mgssd.auth_tokens.create
create table if not exists `auth_tokens`
(
	id						bigint unsigned	not null	auto_increment,
	type					int unsigned	not null,
	account_id				bigint unsigned	not null	 unique,
	token_hash				char(64)		default null unique,
	expire_date				datetime not null,
	primary key (`id`)
)
-- query:mgssd.variables.create
create table if not exists `variables`
(
	id						bigint unsigned	not null	auto_increment,
	variable_name			varchar(256) 	not null	unique,
	variable_value			json			not null,
	primary key (`id`)
)
