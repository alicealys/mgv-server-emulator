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
	last_update				datetime        default null,
	user_creation_date		datetime        default null,
	last_daily_reward		datetime		default null,
	user_flag				int unsigned 	default 0,
	dlc_flag				int unsigned 	default 0,
	sv_coin					int unsigned 	default 0,
	loadout_count			int unsigned default 4,
	event_point				int unsigned not null default 0,
	event_point_total		int unsigned not null default 0,
	player_capacity			bigint unsigned not null default 1,
	user_inventory			blob			default null
)
-- query:mgssd.players.create
create table if not exists `players`
(
	player_id					integer	primary key autoincrement,
	f_user_id					bigint unsigned not null,
	player_index				bigint unsigned unique default null,
	player_creation_date		datetime        not null,
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
	nonstackable_item_list		blob default null,
	battle_pack_list			blob default null,
	map_unlock_list_afghan		blob default null,
	map_unlock_list_africa		blob default null,
	building_info_afghan		blob default null,
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
-- query:mgssd.present_box_entries.create
create table if not exists `present_box_entries`
(
	present_id			integer	primary key autoincrement,
	f_user_id			bigint unsigned	not null,
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
	foreign key (`f_user_id`) references `users`(`user_id`)
)
-- query:mgssd.deployment_teams.create
create table if not exists `deployment_teams`
(
	team_id				integer	primary key autoincrement,
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
	foreign key (`f_player_id`) references `players`(`player_id`),
	foreign key (`crew_id_01`) references `crew_memebers`(`member_id`),
	foreign key (`crew_id_02`) references `crew_memebers`(`member_id`),
	foreign key (`crew_id_03`) references `crew_memebers`(`member_id`),
	foreign key (`crew_id_04`) references `crew_memebers`(`member_id`)
)
-- query:mgssd.deployment_teams.remove_insert_trigger
drop trigger if exists deployment_teams_insert_trigger
-- query:mgssd.deployment_teams.add_insert_trigger
create trigger deployment_teams_insert_trigger
after insert on deployment_teams
for each row
begin
    update deployment_teams
    set team_index = (
        select count(*) - 1 
        from deployment_teams 
        where f_player_id = NEW.f_player_id
    )
    where team_id = NEW.player_id;
end
-- query:mgssd.deployment_teams.remove_update_trigger
drop trigger if exists deployment_teams_update_trigger
-- query:mgssd.deployment_teams.add_update_trigger
create trigger deployment_teams_update_trigger
before update on deployment_teams
for each row
begin
	select raise(abort, 'deployment teams crew member id check fail')
	where ((NEW.crew_id_01 is not null) + (NEW.crew_id_02 is not null) + 
		   (NEW.crew_id_03 is not null) + (NEW.crew_id_04 is not null)) > 
		(select COUNT(*) 
			from crew_members 
			where f_player_id = NEW.f_player_id  and current_group = 7
			and member_id in (NEW.crew_id_01, NEW.crew_id_02, NEW.crew_id_03, NEW.crew_id_04)
	);
end
-- query:mgssd.crew_members.create
create table if not exists `crew_members`
(
	member_id			integer	primary key autoincrement,
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
	health_condition	int unsigned not null default 0,
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
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.shop_purchases.create
create table if not exists `shop_purchases`
(
	purchase_id			    integer	primary key autoincrement,
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
	foreign key (`f_user_id`) references users(`user_id`)
)
-- query:mgssd.wicked_reports.create
create table if not exists `wicked_reports`
(
	report_id			    integer	primary key autoincrement,
	target_user_id			bigint unsigned	not null, -- no fk because user could be deleted and reports should persist
	source_user_id			bigint unsigned	not null, -- ^
	report_type_01			int unsigned not null default 0,
	report_type_02			int unsigned not null default 0,
	report_type_03			int unsigned not null default 0,
	report_type_04			int unsigned not null default 0,
	report_type_05			int unsigned not null default 0,
	report_date				datetime not null
)
-- query:mgssd.challenge_task_orders.create
create table if not exists `challenge_task_orders`
(
	task_order_id			integer	primary key autoincrement,
	f_player_id				bigint unsigned	not null,
	task_id					int unsigned	not null,
	is_complete				boolean not null default false,
	progress				int unsigned	not null default 0,
	expire_date				datetime not null,
	foreign key (`f_player_id`) references `players`(`player_id`),
	unique (`task_id`, `expire_date`)
)
-- query:mgssd.running_events.create
create table if not exists `running_events`
(
	event_id			integer	primary key autoincrement,
	event_type			varchar(64) not null,
	event_state			int unsigned not null default 0,
	start_date			datetime not null,
	end_date			datetime not null
)
-- query:mgssd.event_rewards.create
create table if not exists `event_rewards`
(
	event_reward_id		integer	primary key autoincrement,
	f_event_id			bigint unsigned	not null,
	f_user_id			bigint unsigned	not null,
	reward_type			int unsigned not null,
	reward_id			bigint unsigned not null,
	acquired			boolean not null,
	create_date			datetime not null,
	foreign key (`f_event_id`) references `running_events`(`event_id`),
	foreign key (`f_user_id`) references `users`(`user_id`)
)
-- query:mgssd.rankings.create
create table if not exists `rankings`
(
	ranking_id          integer	primary key autoincrement,
	f_user_id	        bigint unsigned	not null,
	ranking_type	    int unsigned	not null,
	ranking_state	    int unsigned	not null default 0,
	player_rank		    bigint unsigned	not null default 0,
	player_rank_number	bigint unsigned	not null default 0,
	points		        int				not null default 0,
	foreign key (`f_user_id`) references users(`user_id`),
	unique (`f_user_id`, `ranking_type`)
)
-- query:mgssd.rankings.update_entries
with ranked as (
    select
        f_user_id,
        ranking_type,
        points,
        rank() over (partition by ranking_type order by points desc) as new_rank,
        row_number() over (partition by ranking_type order by points desc) as new_rank_number
    from rankings where ranking_type = {}
)
update rankings
set player_rank = (
    select ranked.new_rank
    from ranked, rankings
    where ranked.f_user_id = rankings.f_user_id
      and ranked.ranking_type = rankings.ranking_type
),
player_rank_number = (
    select ranked.new_rank_number
    from ranked, rankings
    where ranked.f_user_id = rankings.f_user_id
      and ranked.ranking_type = rankings.ranking_type
);
-- query:mgssd.matching_rooms.create
create table if not exists `matching_rooms`
(
	room_id					integer	primary key autoincrement,
	owner_player_id			bigint unsigned	not null	unique,
	f_coop_mission_id		bigint unsigned,
	max_slot				int unsigned	not null,
	region_matching_level	int unsigned	not null,
	flag_attr				int unsigned	not null,
	flag_filter				int unsigned	not null,
	status					int unsigned	not null default 0,
	reserve_num				int unsigned	not null default 0,
	password				varchar(64)	default null,
	int_attr_01				bigint not null default 0,
	int_attr_02				bigint not null default 0,
	int_attr_03				bigint not null default 0,
	int_attr_04				bigint not null default 0,
	int_attr_05				bigint not null default 0,
	int_attr_06				bigint not null default 0,
	int_attr_07				bigint not null default 0,
	int_attr_08				bigint not null default 0,
	int_attr_09				bigint not null default 0,
	int_attr_10				bigint not null default 0,
	int_attr_11				bigint not null default 0,
	int_attr_12				bigint not null default 0,
	int_attr_13				bigint not null default 0,
	int_attr_14				bigint not null default 0,
	int_attr_15				bigint not null default 0,
	int_attr_16				bigint not null default 0,
	bin_attr_01				varchar(512) default null,
	bin_attr_02				varchar(512) default null,
	create_date				datetime not null,
	foreign key (`owner_player_id`) references `players`(`player_id`)
)
-- query:mgssd.matching_members.create
create table if not exists `matching_members`
(
	member_id			integer	primary key autoincrement,
	f_room_id			bigint unsigned	not null,
	f_player_id			bigint unsigned	not null	unique,
	migration_sequence	int unsigned	default 0,
	member_data			varchar(256) default null,
	create_date			datetime not null,
	foreign key (`f_room_id`) references `matching_rooms`(`room_id`),
	foreign key (`f_player_id`) references `players`(`player_id`)
)
-- query:mgssd.coop_missions.create
create table if not exists `coop_missions`
(
	coop_mission_id		integer	primary key autoincrement,
	owner_player_id		bigint unsigned	not null,
	mission_code		int unsigned	not null,
	flag				int unsigned	not null,
	create_date			datetime not null,
	primary key (`coop_mission_id`),
	foreign key (`owner_player_id`) references `players`(`player_id`)
)
-- query:mgssd.coop_mission_results.create
create table if not exists `coop_mission_results`
(
	coop_mission_result_id  integer	primary key autoincrement,
	f_player_id				bigint unsigned	not null,
	mission_code			int unsigned	not null,
	mission_type			int unsigned	not null,
	clear_rank				int unsigned	not null,
	score					int unsigned	not null,
	personal_score			int unsigned	not null,
	result					int unsigned	not null,
	waves					int unsigned	not null,
	rescue					int unsigned	not null,
	create_date				datetime not null,
	primary key (`coop_mission_result_id`),
	foreign key (`f_player_id`) references `players`(`player_id`)
)
