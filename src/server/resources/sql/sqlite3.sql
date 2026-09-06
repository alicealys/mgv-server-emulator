-- query:mgssd.users.create
create table if not exists `users`
(
	user_id					integer	primary key autoincrement,
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
	last_update				datetime        not null,
	user_creation_date		datetime        not null
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id				integer	primary key autoincrement,
	f_user_id				bigint unsigned unique,
	player_index			bigint unsigned unique default null,
	player_creation_date	datetime        not null,
	foreign key (`f_user_id`) references users(`user_id`)
)
-- query:mgssd.players.remove_insert_trigger
drop trigger if exists players_insert_trigger
-- query:mgssd.players.add_insert_trigger
create trigger players_insert_trigger
after insert on players
for each row
begin
    update players
    set player_index = (
        select count(*) - 1 
        from players 
        where f_user_id = NEW.f_user_id
    )
    where player_id = NEW.player_id;
end
-- query:mgssd.variables.create
create table if not exists `variables`
(
	id						integer	primary key autoincrement,
	variable_name			varchar(256) 	not null	unique,
	variable_value			json			not null
)
-- query:mgssd.auth_tokens.create
create table if not exists `auth_tokens`
(
	id						integer	primary key autoincrement,
	type					integer	unsigned not null,
	account_id				bigint unsigned	not null	 unique,
	token_hash				char(64)		default null unique,
	expire_date				datetime not null
)
