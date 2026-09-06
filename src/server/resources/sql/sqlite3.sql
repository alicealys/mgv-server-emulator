-- query:mgvdb.users.create
create table if not exists `users`
(
	id						integer	primary key autoincrement,
	account_id				bigint unsigned	not null	 unique,
	session_id				char(32)		default null unique,
	login_password			varchar(32)		default null,
	crypto_key				char(32)		default null,
	currency				varchar(32)		default null,
	current_player_id		bigint unsigned default null,
	last_update				datetime        not null,
	creation_time			datetime        not null
)
-- query:mgvdb.players.create
create table if not exists `players`
(
	id						integer	primary key autoincrement,
	user_id					bigint unsigned unique,
	player_index			bigint unsigned unique default null,
	ex_ip					varchar(15)		default null,
	in_ip					varchar(15)		default null,
	ex_port					int unsigned	default 0,
	in_port					int unsigned	default 0,
	nat						int unsigned	default 0,
	creation_time			datetime        not null,
	foreign key (`user_id`) references users(`id`)
)
-- query:mgvdb.players.remove_insert_trigger
drop trigger if exists players_insert_trigger
-- query:mgvdb.players.add_insert_trigger
create trigger players_insert_trigger
after insert on players
for each row
begin
    update players
    set player_index = (
        select count(*) - 1 
        from players 
        where user_id = NEW.user_id
    )
    where id = NEW.id;
end
-- query:mgvdb.variables.create
create table if not exists `variables`
(
	id						integer	primary key autoincrement,
	variable_name			varchar(256) 	not null	unique,
	variable_value			json			not null
)
-- query:mgvdb.auth_tokens.create
create table if not exists `auth_tokens`
(
	id						integer	primary key autoincrement,
	type					integer	unsigned not null,
	account_id				bigint unsigned	not null	 unique,
	auth_token				char(32)		default null unique,
	expire_date				datetime not null
)
