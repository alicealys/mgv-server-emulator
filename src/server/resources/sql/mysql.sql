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
	last_daily_reward		datetime		default null,
	user_flag				int unsigned 	default 0,
	dlc_flag				int unsigned 	default 0,
	sv_coin					int unsigned 	default 0,
	loadout_count			int unsigned default 4,
	player_capacity			bigint unsigned not null default 1,
	user_inventory			blob			default null,
	primary key (`user_id`)
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id					bigint unsigned	not null	auto_increment,
	f_user_id					bigint unsigned not null,
	player_index				bigint unsigned default 0,
	player_creation_date		datetime not null,
	nameplate					int unsigned default 0,
	gimmick_info				varchar(64) default null,
	crew_levels					varchar(64) default null,
	defense_mission_info		varchar(64) default null,
	communication_gesture_info	varchar(64) default null,
	mission_record_list  		varchar(64) default null,
	replay_info_list	  		varchar(64) default null,
	avatar						varchar(256) default null,
	quest_record_list  			varchar(480) default null,
	loadout_list				blob default null,
	mission_info				blob default null,
	player_inventory			blob default null,
	gimmick_data_afghan			blob default null,
	gimmick_data_africa			blob default null,
	player_play_record			blob default null,
	base_resources  			blob default null,
	story_unlock_info  			blob default null,
	stackable_item_list  		blob default null,
	inventory_resource_list 	blob default null,
	battle_pack_list		 	blob default null,
	map_unlock_list_afghan		blob default null,
	map_unlock_list_africa		blob default null,
	nonstackable_item_list		mediumblob default null,
	building_info_afghan		mediumblob default null,
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
-- query:mgssd.defense_missions.create
create table if not exists `defense_missions`
(
	defense_mission_id	bigint unsigned	not null	auto_increment,
	f_player_id			bigint unsigned	not null,
	mission_code		int unsigned	not null,
	current_wave		int unsigned	default 0,
	clear_rank			int unsigned	default 0,
	result				int unsigned	default 0,
	total_score			int unsigned	default 0,
	start_date			datetime not null,
	next_wave_date		datetime not null,
	end_date			datetime default null,
	primary key (`defense_mission_id`),
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.defense_mission_waves.create
create table if not exists `defense_mission_waves`
(
	defense_mission_wave_id	bigint unsigned	not null	auto_increment,
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
	primary key (`defense_mission_wave_id`),
	foreign key (`f_defense_mission_id`) references `defense_missions`(`defense_mission_id`)
)
-- query:mgssd.present_box_entries.create
create table if not exists `present_box_entries`
(
	present_id			bigint unsigned not null	auto_increment,
	f_player_id			bigint unsigned	not null,
	flags				int unsigned not null default 0,
	item_category		int unsigned not null default 0,
	item_code			int unsigned not null default 0,
	item_num			int unsigned not null default 0,
	item_param1			int unsigned not null default 0,
	item_param2			int unsigned not null default 0,
	item_param3			int unsigned not null default 0,
	item_param4			int unsigned not null default 0,
	item_param5			int unsigned not null default 0,
	text_id				bigint unsigned not null default 0,
	expire_date			datetime not null,
	primary key (`present_id`),
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.deployment_teams.create
create table if not exists `deployment_teams`
(
	team_id				bigint unsigned	not null	auto_increment,
	f_player_id			bigint unsigned	not null,
	team_index			bigint unsigned	not null,
	params				varchar(64) default null,
	info_name			varchar(64) default null,
	info_status			int unsigned not null default 0,
	info_combat			int unsigned not null default 0,
	info_survive		int unsigned not null default 0,
	mission_id			int unsigned not null default 0,
	mission_type		int unsigned not null default 0,
	mission_info		int unsigned not null default 0,
	crew_id_01			bigint unsigned default null unique,
	crew_id_02			bigint unsigned default null unique,
	crew_id_03			bigint unsigned default null unique,
	crew_id_04			bigint unsigned default null unique,
	item_01				bigint unsigned not null default 0,
	item_02				bigint unsigned not null default 0,
	item_03				bigint unsigned not null default 0,
	item_04				bigint unsigned not null default 0,
	item_05				bigint unsigned not null default 0,
	complete_date		datetime not null,
	creation_date		datetime not null,
	primary key (`team_id`),
	foreign key (`f_player_id`) references `players`(`player_id`),
	foreign key (`crew_id_01`) references `crew_members`(`member_id`),
	foreign key (`crew_id_02`) references `crew_members`(`member_id`),
	foreign key (`crew_id_03`) references `crew_members`(`member_id`),
	foreign key (`crew_id_04`) references `crew_members`(`member_id`)
)
-- query:mgssd.deployment_teams.remove_insert_trigger
drop trigger if exists deployment_teams_insert_trigger
-- query:mgssd.deployment_teams.add_insert_trigger
create trigger deployment_teams_insert_trigger
before insert on deployment_teams
for each row
begin
	declare total_rows int;
    select count(*) into total_rows 
	from deployment_teams where f_player_id = NEW.f_player_id;
	set NEW.team_index = total_rows;
end
-- query:mgssd.deployment_teams.remove_update_trigger
drop trigger if exists deployment_teams_update_trigger
-- query:mgssd.deployment_teams.add_update_trigger
create trigger deployment_teams_update_trigger
before update on deployment_teams
for each row
begin
	if (((NEW.crew_id_01 is not null) + (NEW.crew_id_02 is not null) + 
	     (NEW.crew_id_03 is not null) + (NEW.crew_id_04 is not null)) > 
		(select COUNT(*) 
			from crew_members 
			where f_player_id = NEW.f_player_id and current_group = 7
			and member_id in (NEW.crew_id_01, NEW.crew_id_02, NEW.crew_id_03, NEW.crew_id_04))
	) then 
		signal sqlstate '45000' 
		set message_Text = 'deployment teams crew member id check fail';
	end if;
end
-- query:mgssd.crew_members.create
create table if not exists `crew_members`
(
	member_id			bigint unsigned	not null	auto_increment,
	f_player_id			bigint unsigned	not null,
	member_type 		int unsigned not null default 0,
	face_id				int unsigned not null default 0,
	body_id				int unsigned not null default 0,
	race_id				int unsigned not null default 0,
	sex_id				int unsigned not null default 0,
	voice_type			int unsigned not null default 0,
	previous_group		int unsigned not null default 0,
	current_group		int unsigned not null default 0,
	previous_job		int unsigned not null default 0,
	item1_count			int unsigned not null default 0,
	item2_count			int unsigned not null default 0,
	item3_count			int unsigned not null default 0,
	item4_count			int unsigned not null default 0,
	item5_count			int unsigned not null default 0,
	item6_count			int unsigned not null default 0,
	life				int unsigned not null default 0,
	life_max			int unsigned not null default 0,
	health_condition    int unsigned not null default 0,
	health_flag			int unsigned not null default 0,
	map_location		int unsigned not null default 0,
	skill				int unsigned not null default 0,
	sanity				int unsigned not null default 0,
	survival_days		int unsigned not null default 0,
	injury_id_1			int unsigned not null default 0,
	injury_id_2			int unsigned not null default 0,
	injury_time_1		int unsigned not null default 0,
	injury_time_2		int unsigned not null default 0,
	sickness_id_1		int unsigned not null default 0,
	sickness_id_2		int unsigned not null default 0,
	sickness_time_1		int unsigned not null default 0,
	sickness_time_2		int unsigned not null default 0,
	nickname			varchar(64) default null,
	motivation_history	varchar(64) default null,
	creation_date	    datetime not null,
	primary key (`member_id`),
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.shop_purchases.create
create table if not exists `shop_purchases`
(
	purchase_id			    bigint unsigned	not null	auto_increment,
	f_user_id				bigint unsigned	not null,
	lang_id					bigint unsigned	not null default 0,
	event_type				int unsigned not null default 0,
	product_type			int unsigned not null default 0,
	purchase_quantity		int unsigned not null default 0,
	coin_quantity			int unsigned not null default 0,
	remaining_coin			int unsigned not null default 0,
	item_category			int unsigned not null default 0,
	item_code				int unsigned not null default 0,
	item_param1				int unsigned not null default 0,
	item_param2				int unsigned not null default 0,
	item_param3				int unsigned not null default 0,
	item_param4				int unsigned not null default 0,
	item_param5				int unsigned not null default 0,
	item_num				int unsigned not null default 0,
	purchase_date           datetime not null,
	expire_date				datetime not null,
	primary key (`purchase_id`),
	foreign key (`f_user_id`) references users(`user_id`)
)
-- query:mgssd.wicked_reports.create
create table if not exists `wicked_reports`
(
	report_id			    bigint unsigned	not null	auto_increment,
	target_user_id			bigint unsigned	not null, -- no fk because user could be deleted and reports should persist
	source_user_id			bigint unsigned	not null, -- ^
	report_type_01			int unsigned not null default 0,
	report_type_02			int unsigned not null default 0,
	report_type_03			int unsigned not null default 0,
	report_type_04			int unsigned not null default 0,
	report_type_05			int unsigned not null default 0,
	report_date				datetime not null,
	primary key (`report_id`)
)
-- query:mgssd.challenge_task_orders.create
create table if not exists `challenge_task_orders`
(
	task_order_id			bigint unsigned	not null	auto_increment,
	f_player_id				bigint unsigned	not null,
	task_id					int unsigned	not null,
	is_complete				boolean not null default false,
	progress				int unsigned	not null default 0,
	expire_date				datetime not null,
	primary key (`task_order_id`),
	foreign key (`f_player_id`) references `players`(`player_id`),
	constraint unique_task_id unique (`task_id`, `expire_date`)
)
