#include <std_include.hpp>

#include "battle_packs.hpp"
#include "game/parameters.hpp"

#include <utils/cryptography.hpp>

namespace game
{
	namespace
	{
		std::uint32_t rand_index(const std::vector<std::uint32_t>& pool)
		{
			if (pool.size() == 0)
			{
				throw std::runtime_error("[battle packs] item pool empty");
			}

			return utils::cryptography::random::get_integer(0, static_cast<std::uint32_t>(pool.size() - 1));
		}

		std::vector<std::uint32_t> list_resources(const std::uint32_t rarity, bool (*filter)(const game::resource_t&))
		{
			const auto& resources_list = game::parameters_table.ssd_sbm_parameters->resources_list;

			std::vector<std::uint32_t> result{};

			for (auto i = 0ull; i < resources_list.size(); i++)
			{
				const auto& prod = resources_list[i];

				if (prod != nullptr && prod->rarity == rarity && filter(*prod))
				{
					result.emplace_back(prod->id);
				}
			}

			return result;
		}

		std::vector<std::uint32_t> list_productions(bool (*filter)(const game::recipe_t&))
		{
			const auto& recipes_list = game::parameters_table.ssd_sbm_parameters->recipes_list;

			std::vector<std::uint32_t> result{};

			for (auto i = 0ull; i < recipes_list.size(); i++)
			{
				const auto& prod = recipes_list[i];

				if (prod != nullptr && filter(*prod))
				{
					result.emplace_back(prod->production->id);
				}
			}

			return result;
		}

		template <std::uint32_t Rarity>
		game::item_t lottery_material()
		{
			static const auto pool = list_resources(Rarity, [](const game::resource_t& resource)
			{
				static std::unordered_set<std::uint32_t> set =
				{
					MATERIAL_BUILDING,
					MATERIAL_CHEMICAL,
					MATERIAL_CIRCUIT,
					MATERIAL_FABRIC,
					MATERIAL_METAL,
					MATERIAL_PARTS,
					MATERIAL_WOOD,
				};

				return set.contains(resource.material) || Rarity >= 4;
			});

			console::debug("[battle packs] lottery_material %i pool size: %lli\n", Rarity, pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RESOURCE;
			item.num = utils::cryptography::random::get_integer(0, 10);

			const auto idx = rand_index(pool);
			item.code = pool[idx];
			
			return item;
		}

		game::item_t lottery_crew_growth()
		{
			static const auto pool = list_resources(5, [](const game::resource_t& resource)
			{
				return resource.lang_name == 115687727752924;
			});

			console::debug("[battle packs] lottery_crew_growth pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RESOURCE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Rarity>
		game::item_t lottery_junk_weapon()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != Rarity || !recipe.junk)
				{
					return false;
				}

				const auto equip = recipe.production->get_equip();
				if (equip == nullptr)
				{
					return false;
				}

				static std::unordered_set<std::uint32_t> equip_types =
				{
					EQP_TYPE_AttackArm,
					EQP_TYPE_Hammer,
					EQP_TYPE_Katana,
					EQP_TYPE_Shield,
					EQP_TYPE_Slash,
					EQP_TYPE_Slash_Two,
					EQP_TYPE_Throwing,
					EQP_TYPE_Thrust,
					EQP_TYPE_Thrust_Two,
					EQP_TYPE_Assault,
					EQP_TYPE_Bow,
					EQP_TYPE_GrenadeLauncher,
					EQP_TYPE_Handgun,
					EQP_TYPE_Machinegun,
					EQP_TYPE_Shotgun,
					EQP_TYPE_Sniper,
					EQP_TYPE_Submachinegun
				};

				return equip_types.contains(equip->type);
			});

			console::debug("[battle packs] lottery_junk_weapon %i pool size: %lli\n", Rarity, pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Rarity>
		game::item_t lottery_junk_accessory()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				return recipe.production != nullptr && recipe.production->type == PRD_TYPE_Accessory &&
					recipe.production->rarity == Rarity;
			});

			console::debug("[battle packs] lottery_junk_accessory pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		game::item_t lottery_junk_weapon_legendary_closerange()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != 4 || !recipe.junk)
				{
					return false;
				}

				const auto equip = recipe.production->get_equip();
				if (equip == nullptr)
				{
					return false;
				}

				static std::unordered_set<std::uint32_t> equip_types =
				{
					EQP_TYPE_AttackArm,
					EQP_TYPE_Hammer,
					EQP_TYPE_Katana,
					EQP_TYPE_Shield,
					EQP_TYPE_Slash,
					EQP_TYPE_Slash_Two,
					EQP_TYPE_Throwing,
					EQP_TYPE_Thrust,
					EQP_TYPE_Thrust_Two
				};

				return equip_types.contains(equip->type);
			});

			console::debug("[battle packs] lottery_junk_weapon_legendary_closerange pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		game::item_t lottery_junk_weapon_legendary_longrange()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != 4 || !recipe.junk)
				{
					return false;
				}

				const auto equip = recipe.production->get_equip();
				if (equip == nullptr)
				{
					return false;
				}

				static std::unordered_set<std::uint32_t> equip_types =
				{
					EQP_TYPE_Assault,
					EQP_TYPE_Bow,
					EQP_TYPE_GrenadeLauncher,
					EQP_TYPE_Handgun,
					EQP_TYPE_Machinegun,
					EQP_TYPE_Shotgun,
					EQP_TYPE_Sniper,
					EQP_TYPE_Submachinegun
				};

				return equip_types.contains(equip->type);
			});

			console::debug("[battle packs] lottery_junk_weapon_legendary_longrange pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Type>
		game::item_t lottery_junk_accessory_legendary()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Accessory || recipe.production->rarity != 4)
				{
					return false;
				}

				const auto accessory = recipe.production->get_accessory();
				if (accessory == nullptr)
				{
					return false;
				}

				static std::unordered_set<std::uint32_t> accessory_types =
				{
					EQP_TYPE_Assault,
					EQP_TYPE_Bow,
					EQP_TYPE_GrenadeLauncher,
					EQP_TYPE_Handgun,
					EQP_TYPE_Machinegun,
					EQP_TYPE_Shotgun,
					EQP_TYPE_Sniper,
					EQP_TYPE_Submachinegun
				};

				return accessory->typeId == Type;
			});

			console::debug("[battle packs] lottery_junk_accessory_legendary %i pool size: %lli\n", Type, pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Production>
		game::item_t lottery_production_single()
		{
			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.code = Production;
			item.num = 1;

			return item;
		}

		game::item_t lottery_weapon_enhance()
		{
			static const auto pool = list_resources(5, [](const game::resource_t& resource)
			{
				return resource.material == MATERIAL_ENHANCEMENT;
			});

			console::debug("[battle packs] lottery_weapon_enhance pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RESOURCE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Amount>
		game::item_t lottery_energy()
		{
			game::item_t item{};
			item.category = game::ITEM_CATEGORY_ENERGY;
			item.code = 0;
			item.num = Amount;

			return item;
		}

		game::item_t lottery_medical()
		{
			static const auto pool = list_productions([](const game::recipe_t& recipe)
			{
				return recipe.production != nullptr && recipe.production->type == PRD_TYPE_Cure && recipe.production->rarity == 1;
			});

			console::debug("[battle packs] lottery_medical pool size: %lli\n", pool.size());

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_PRODUCTION;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}
	}

	std::unordered_map<std::uint32_t, battle_pack_lottery_t> battle_pack_lotteries;

	void register_battle_pack_lottery(const std::uint32_t type, const battle_pack_lottery_t& lottery)
	{
		lottery();
		battle_pack_lotteries.insert(std::make_pair(type, lottery));
	}

	void initialize_battle_packs()
	{
		register_battle_pack_lottery(BP_Material_Common, lottery_material<1>);
		register_battle_pack_lottery(BP_Material_Uncommon, lottery_material<2>);
		register_battle_pack_lottery(BP_Material_Rare, lottery_material<3>);
		register_battle_pack_lottery(BP_Material_Legendary, lottery_material<4>);
		register_battle_pack_lottery(BP_Material_Epic, lottery_material<5>);
		register_battle_pack_lottery(BP_Material_Crew_Growth, lottery_crew_growth);
		register_battle_pack_lottery(BP_Junk_Weapon_Common, lottery_junk_weapon<1>);
		register_battle_pack_lottery(BP_Junk_Weapon_Uncommon, lottery_junk_weapon<2>);
		register_battle_pack_lottery(BP_Junk_Weapon_Rare, lottery_junk_weapon<3>);
		register_battle_pack_lottery(BP_Junk_Weapon_Legendary, lottery_junk_weapon<4>);
		register_battle_pack_lottery(BP_Junk_Weapon_Epic, lottery_junk_weapon<5>);
		register_battle_pack_lottery(BP_Junk_Accessory_Common, lottery_junk_accessory<1>);
		register_battle_pack_lottery(BP_Junk_Accessory_Uncommon, lottery_junk_accessory<2>);
		register_battle_pack_lottery(BP_Junk_Accessory_Rare, lottery_junk_accessory<3>);
		register_battle_pack_lottery(BP_Junk_Accessory_Legendary, lottery_junk_accessory<4>);
		register_battle_pack_lottery(BP_Junk_Accessory_Epic, lottery_junk_accessory<5>);
		register_battle_pack_lottery(BP_Junk_Weapon_Legendary_CloseRange, lottery_junk_weapon_legendary_closerange);
		register_battle_pack_lottery(BP_Junk_Weapon_Legendary_LongRange, lottery_junk_weapon_legendary_longrange);
		register_battle_pack_lottery(BP_Junk_Accessory_Legendary_Head, lottery_junk_accessory_legendary<ACC_TYPE_Head>);
		register_battle_pack_lottery(BP_Junk_Accessory_Legendary_Body, lottery_junk_accessory_legendary<ACC_TYPE_Body>);
		register_battle_pack_lottery(BP_Junk_Accessory_Legendary_Arm, lottery_junk_accessory_legendary<ACC_TYPE_Arm>);
		register_battle_pack_lottery(BP_Junk_Accessory_Legendary_Leg, lottery_junk_accessory_legendary<ACC_TYPE_Leg>);
		register_battle_pack_lottery(BP_Junk_Weapon_Legendary_singleQuest, lottery_production_single<PRD_EQP_WP_sg01_sw1>);
		register_battle_pack_lottery(BP_Boss01Reward_Weapon, lottery_production_single<PRD_EQP_WP_sr03>);
		register_battle_pack_lottery(BP_Boss01Reward_Accessory, lottery_production_single<PRD_ACC_Body_15>);
		register_battle_pack_lottery(BP_Boss01Reward_Material, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss02Reward_WeaponEnhance, lottery_weapon_enhance);
		register_battle_pack_lottery(BP_Boss02Reward_AccessoryEnhance, lottery_weapon_enhance);
		register_battle_pack_lottery(BP_Boss02Reward_Material, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss03Reward_01, lottery_production_single<PRD_ACC_Body_15>);
		register_battle_pack_lottery(BP_Boss03Reward_02, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss03Reward_03, lottery_material<2>);
		register_battle_pack_lottery(BP_Boss04Reward_01, lottery_material<4>);
		register_battle_pack_lottery(BP_Boss04Reward_02, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss04Reward_03, lottery_material<2>);
		register_battle_pack_lottery(BP_KubanEnergy_100K, lottery_energy<100'000>);
		register_battle_pack_lottery(BP_Medical_Kit, lottery_medical);
	}
}
