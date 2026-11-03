# Usage

## Installation

- Download binary 
  - **[Windows x64](https://github.alicent.cat/mgv-server-emulator/mgv-server-emulator-windows_x64.tar)**  
  - **[Linux x64](https://github.alicent.cat/mgv-server-emulator/mgv-server-emulator-linux_x64.tar)**  
- \***OPTIONAL**\* Setup a mysql server with a database called `mgssd`.
- \***OPTIONAL**\* In the EXE's path create a file called `config.json` and add the following values:
    * `database_type`: (default: `"mysql"`, options: "mysql", "sqlite3")
    * `database_user`: (default: `"root"`)
    * `database_password`: (default: `"root"`)
    * `database_host`: (default: `"localhost"`)
    * `database_port`: (default: `3306`)
    * `database_name`: (default: `"mgssd"`)
- Run `mgv-server-emulator.exe` or run `start.sh` for linux to start the server
- Run [mgv-mod](https://github.com/alicealys/mgv-mod) with the `+net_ncl_url <url>` command line param.  
  `<url>` is the url to the gate endpoint of the server, default: `http://localhost/ssdstm/gate`

## Config

Example config

```json
{
    "base_url": "http://localhost:80",
    "http_port": 80,
    "database_type": "sqlite3",
    "database_password": "root",
    "database_name": "mgssd",
    "auth_mode": "offline",
    "vars": {
        "session_heartbeat": 60,
        "session_timeout": 200
    }
}
```

### Config values

(`vars.*` values must be defined in a `"vars"` object)

| name | desc | default |
| --- | --- | --- |
| base_url | base url for the url list command | http://localhost:80 |
| http_port | https port | 80 |
| https_port | https port | 443 |
| cert_file | file path for ssl certificate | "" |
| key_file | file path for tls key | "" |
| database_type | database type (mysql, sqlite3) | "mysql" |
| database_user | database username | "root" |
| database_password | database password | "root" |
| database_host | database hostname | "localhost" |
| database_port | database port | 3306 |
| database_name | database name | "mgsmgv" |
| auth_mode | authentication mode, must be one of: "nosteam", "offline", "konami", "custom", "hybrid", **if set to `nosteam` steam tickets will not be verified** | "offline" |
| steam_api_key | steam api key used for retrieval of player names | "" |
| steam_public_key | valve's public key used for auth session tickets encoded in base64 | N/A |
| max_worker_threads | max http server worker threads | 100 |
| max_database_connections | max database connections | 100 |
| vars.session_heartbeat | interval between each CMD_UPDATE_SESSION sent by the client (in seconds) | 60 |
| vars.session_timeout | time until client is considered offline (seconds) | 200 |
| vars.server_version | server version | 19 |
| vars.disable_tutorial_tips | disable tutorial tips | false |
| vars.reference_time | reference unix timestamp used for time periods calculation | 1519113600 |
| vars.store_client_ips | store IP addresses sent by players during log-in | false |
| vars.allow_save_restore | enable CMD_RESTORE_SAVE command (enables `dumpsave`/`restoresave` functionality from mgv-mod) | true |

### Authentication mode

mgv-server-emulator allows for 4 different authentication modes to verify players that log in:

- **offline** (default): steam ticket is parsed and using the App Ticket section:
    - validates its signature against Valve's public RSA key
    - checks that the App Id matches the game's (543900)
    - checks steam id format
    - checks that the ticket isn't expired
- **konami**: steam ticket is relayed to KONAMI servers and verified.
- **nosteam**: no verification is done, steam id is extracted from the GC section of the steam ticket.
- **custom**: requires a custom steam ticket, and for the user to have linked their steam account to the server.  
    the structure for the ticket is the following:
    ```c
    struct auth_ticket_custom_t
    {
    	std::uint64_t account_id; // steam id
    	char auth_token[32]; 
    }; // sizeof(auth_ticket_custom_t) == 40
    ```
    The auth token must be generated using Steam OpenID, by setting "/api/steam_openid" as return address.   
    This API will return the following response if valid:
    ```json
    {
        "is_valid": true,
        "account_id": 76561197960287930,
        "auth_token": "0123456789ABCDEF0123456789ABCDEF"
    }
    ```

    The auth token will be valid for 365 days, or until a new one is generated.

    The game client must then send a ticket with this information using the format described above in the **CMD_AUTH_STEAMTICKET** request.  
    An example of the redirect URL you should provide the client:

    ```
    https://steamcommunity.com/openid/login?openid.ns=http://specs.openid.net/auth/2.0&
    openid.claimed_id=http://specs.openid.net/auth/2.0/identifier_select&
    openid.identity=http://specs.openid.net/auth/2.0/identifier_select&
    openid.return_to=http://your-server/api/steam_openid&
    openid.realm=https://your-server&
    openid.mode=checkid_setup
    ```
- **hybrid**: automatically chooses **konami** or **custom** authentication based on ticket size.

## Web API

mgv-server-emulator exposes some web APIs that can be accessed through regular HTTP requests (not using the game's protocol or format).  
The API is enabled by default, but can be disabled by setting `enable_web_api` to `false` in the config.  
All APIs return in JSON format.  

### API list

- `api/steam_openid?{Steam OpenID redirect url}`: generates a new auth token from a valid Steam OpenID redirect url.
- `api/steam_openid_url?target={target endpoint}`: formats the Steam OpenID redirect url given a target api.
- `api/status`: returns some server status information (player count, current event, uptime, ...).
- `api/delete_account?{Steam OpenID redirect url}`: allows a user to delete all their data, requires a valid Steam OpenID redirect url.

## Resource files

Files in https://github.com/alicealys/mgv-server-emulator/tree/main/src/server/resources/data can be overridden if they exist on the disk.  
These files contain data that is sent to the client.  
Example file structure:

```
[root]
├── mgv-server-emulator.exe
├── config.json
├── allow_list.json
├── deny_list.json
├── libcrypto-3-x64.dll
├── libmysql.dll
├── libssl-3-x64.dll
└── resources
    └── data 
        └── ...
```

Most resource files should not be modified, however some can be customized to your liking.
Feel free to explore the rest of the files and tweak any setting you want.

## Parameter files

Within the above specified resource folder there are parameter files which have been dumped from Konami servers and contain metadata on missions, weapons, crew, npc, and various other things.  

| name | description |
| --- | --- |
| DefenseGameBaseDigging.json | defense mission waves strategy metadata |
| DefenseGameEmbeddedMission.json | ^ for coop missions |
| DefenseGameEventMission.json | ^ for event coop missions |
| DefenseGameQuest.json | metadata about quests in coop missions |
| SsdBaseDefenseSettings.json | base defense mission reward and wave lists, wave intervals |
| SsdBuffParameter.json | weapon buffs? |
| SsdBuffParameter_debug.json | ^ |
| SsdBuffParameter_opt.json | ^ |
| SsdBuildingParameterTable.json | contains metadata about base buildings |
| SsdCollectionItems.json | N/A |
| SsdCombatDeployParameterTable.json | N/A |
| SsdCrewGeneratorTable.json | story crew member type list and crew skill tables/values |
| SsdDamageParameter.json | weapon damage parameters |
| SsdDamageParameter_debug.json | ^ |
| SsdDamageParameter_opt.json | ^ |
| SsdDlcMissionSettings.json | daily/weekly/event coop mission list (rotates every 2 weeks) |
| SsdEquipParameters.json | weapons |
| SsdEquipParameters_debug.json | ^ |
| SsdEquipParameters_opt.json | ^ |
| SsdNpcLevelParameters.json | N/A |
| SsdPlayerParameters.json | N/A |
| SsdReplayMissionInfo_opt.json | replay mission list |
| SsdSbmParameters.json | recipe list, item list, building list, customization, etc... |
| SsdSbmParameters_debug.json | ^ |
| SsdSbmParameters_opt.json | ^ |
| SsdUiCoopMissionInfo.json | ui rank and reward list for coop missions |
| SsdWalkerGearParameters.json | N/A |
| SsdWeaponParameters.json | N/A |
| SsdWeaponParameters_debug.json | ^ |
| SsdWeaponParameters_opt.json | ^ |

These files can all be tweaked in various ways to adjust and customize the gameplay experience.  

## Allow/Deny lists

You can filter players connecting to your server using by creating `allow_list.json` or `deny_list.json` in your server folder  
These files must contain an array of steam ids to either allow or deny  
If the allow list doesn't exist all players will be allowed, if its an empty array no one will be allowed  
If the deny list doesn't exist or is empty no players will be denied  
Deny list takes priority over the allow list  

Example:

`allow_list.json`
```json
[
   76560000000001337,
   76560000000001339,
]
```

## Commands

- `reload_lists`: Reloads allow/deny lists
- `query`: Runs an SQL query (mysql only)
- `quit`: Shuts down the server
- `download_parameters`: Downloads updated parameter files from Konami servers

## Command line params

- `-config <path>`: sets the config file path
- `-working_dir <path>`: sets the working directory path
