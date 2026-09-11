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
	current_player_id		bigint unsigned default null unique,
	last_update				datetime		default null,
	user_creation_date		datetime		default null,
	user_flag				int unsigned 	default 0,
	dlc_flag				int unsigned 	default 0,
	user_inventory			blob			default null,
	user_play_record		blob			default null,
	primary key (`user_id`)
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id				bigint unsigned	not null	auto_increment,
	f_user_id				bigint unsigned unique,
	player_index			bigint unsigned default 0,
	player_creation_date	datetime        not null,
	playtime				int unsigned default 0,
	point					int unsigned default 0,
	nameplate				int unsigned default 0,
	current_loadout			int unsigned default 0,
	loadout_count			int unsigned default 4,
	avatar					blob default null,
	loadout_list			blob default null,
	mission_info			blob default null,
	player_inventory		blob default null,
	gimmick_info			blob default null,
	gimmick_data_afghan		blob default null,
	gimmick_data_africa		blob default null,
	player_play_record		blob default null,
	base_resources  		blob default null,
	story_unlock_info  		blob default null,
	mission_record_list  	blob default null,
	stackable_item_list  	blob default null,
	inventory_resource_list blob default null,
	nonstackable_item_list	mediumblob default null,
	map_unlock_list_afghan	mediumblob default null,
	map_unlock_list_africa	mediumblob default null,
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
-- query:mgssd.users.add_player_foreign_key
alter table users
add constraint current_player_id_fk 
foreign key (`current_player_id`) references `players`(`player_id`)
-- query:mgssd.users.remove_update_trigger
drop trigger if exists users_update_trigger
-- query:mgssd.users.add_update_trigger
create trigger users_update_trigger
before update on users
for each row
begin
	if NEW.current_player_id is not null then
		if not exists (
			select 1 from players where 
			player_id = NEW.current_player_id and f_user_id = NEW.user_id
		) then 
			signal sqlstate '45000' set message_text = 'current_player_id check fail';
		end if;
	end if;
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
