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
	current_player_id		bigint unsigned default null unique,
	last_update				datetime        not null,
	user_creation_date		datetime        not null,
	user_flag				int unsigned 	default 0,
	dlc_flag				int unsigned 	default 0,
	user_inventory			blob			default null,
	user_play_record		blob			default null
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id					integer	primary key autoincrement,
	f_user_id					bigint unsigned unique,
	player_index				bigint unsigned unique default null,
	player_creation_date		datetime        not null,
	playtime					int unsigned default 0,
	point						int unsigned default 0,
	nameplate					int unsigned default 0,
	current_loadout				int unsigned default 0,
	loadout_count				int unsigned default 4,
	crew_levels					blob default null,
	avatar						blob default null,
	loadout_list				blob default null,
	mission_info				blob default null,
	player_inventory			blob default null,
	gimmick_info				blob default null,
	gimmick_data_afghan			blob default null,
	gimmick_data_africa			blob default null,
	player_play_record			blob default null,
	base_resources  			blob default null,
	story_unlock_info  			blob default null,
	mission_record_list  		blob default null,
	quest_record_list  			blob default null,
	stackable_item_list  		blob default null,
	inventory_resource_list 	blob default null,
	crew_member_list			blob default null,
	nonstackable_item_list		blob default null,
	map_unlock_list_afghan		blob default null,
	map_unlock_list_africa		blob default null,
	building_info_afghan		blob default null,
	building_info_africa		blob default null,
	defense_mission_info		blob default null,
	defense_mission_record_list	blob default null,
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
    select raise(fail, 'current_player_id check fail')
    where NEW.current_player_id is not null and not exists (
        select 1 from players 
        where player_id = NEW.current_player_id and f_user_id = NEW.user_id
    );
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
-- query:mgssd.defense_missions.create
create table if not exists `defense_missions`
(
	defense_mission_id	integer	primary key autoincrement,
	f_player_id			bigint unsigned	not null,
	mission_code		int unsigned	not null,
	current_wave		int unsigned	default 0,
	clear_rank			int unsigned	default 0,
	result				int unsigned	default 0,
	total_score			int unsigned	default 0,
	start_date			datetime not null,
	next_wave_date		datetime not null,
	end_date			datetime not null,
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.defense_mission_waves.create
create table if not exists `defense_mission_waves`
(
	defense_mission_wave_id	integer	primary key autoincrement,
	f_defense_mission_id	bigint unsigned	not null,
	wave					int unsigned	not null,
	total_score				int unsigned	not null,
	result					int unsigned	not null,
	start_date				datetime not null,
	end_date				datetime not null,
	params					blob default null,
	injury_crew_list		blob default null,
	broken_facility_list	blob default null,
	reward_list				blob default null,
	foreign key (`f_defense_mission_id`) references `defense_missions`(`defense_mission_id`)
)
