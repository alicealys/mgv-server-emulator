#include <std_include.hpp>

#include "lottery.hpp"
#include "game/parameters.hpp"

#include "utils/random.hpp"

namespace game
{
	std::unordered_map<std::uint32_t, lottery_t> battle_pack_lotteries;
	std::array<std::unordered_map<std::uint32_t, lottery_t>, 6> ui_reward_lotteries;

	namespace
	{
		std::size_t rand_index(const std::vector<std::uint32_t>& pool)
		{
			if (pool.size() == 0)
			{
				throw std::runtime_error("[battle packs] item pool empty");
			}

			return utils::random::index(pool);
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

			std::vector<std::uint32_t> result;
			std::unordered_set<std::uint32_t> productions;

			for (auto i = 0ull; i < recipes_list.size(); i++)
			{
				const auto& prod = recipes_list[i];

				if (prod != nullptr && filter(*prod) && !productions.contains(prod->production->id))
				{
					productions.insert(prod->production->id);
					result.emplace_back(prod->production->id);
				}
			}

			return result;
		}

		std::vector<std::uint32_t> list_stackable_items(const std::uint32_t rarity, bool (*filter)(const game::production_t&))
		{
			const auto& productions_list = game::parameters_table.ssd_sbm_parameters->productions_list;

			std::vector<std::uint32_t> result;
			std::unordered_set<std::uint32_t> productions;

			for (auto i = 0ull; i < productions_list.size(); i++)
			{
				const auto& prod = productions_list[i];

				if (prod != nullptr && prod->rarity == rarity && prod->is_stackable() && filter(*prod) && !productions.contains(prod->id))
				{
					productions.insert(prod->id);
					result.emplace_back(prod->id);
				}
			}

			return result;
		}

		std::vector<std::uint32_t> list_recipes(bool (*filter)(const game::recipe_t&))
		{
			const auto& recipes_list = game::parameters_table.ssd_sbm_parameters->recipes_list;

			std::vector<std::uint32_t> result;
			std::unordered_set<std::uint32_t> productions;

			for (auto i = 0ull; i < recipes_list.size(); i++)
			{
				const auto& recipe = recipes_list[i];
				if (recipe != nullptr && filter(*recipe) && !productions.contains(recipe->production_id))
				{
					productions.insert(recipe->production_id);
					result.emplace_back(recipe->id);
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
					MATERIAL_ENHANCEMENT,
				};

				return set.contains(resource.material) && resource.type == PRD_TYPE_Material;
			});

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RESOURCE;
			item.num = utils::random::get_integer(1, 10);

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
			static const auto pool = list_recipes([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != Rarity || !recipe.junk)
				{
					return false;
				}

				const auto& equip = recipe.production->equip;
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

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Rarity>
		game::item_t lottery_junk_accessory()
		{
			static const auto pool = list_recipes([](const game::recipe_t& recipe)
			{
				return recipe.production != nullptr && recipe.production->type == PRD_TYPE_Accessory &&
					recipe.production->rarity == Rarity && recipe.junk;
			});

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		game::item_t lottery_junk_weapon_legendary_closerange()
		{
			static const auto pool = list_recipes([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != 4 || !recipe.junk)
				{
					return false;
				}

				const auto& equip = recipe.production->equip;
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

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		game::item_t lottery_junk_weapon_legendary_longrange()
		{
			static const auto pool = list_recipes([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Weapon || recipe.production->rarity != 4 || !recipe.junk)
				{
					return false;
				}

				const auto& equip = recipe.production->equip;
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

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Type>
		game::item_t lottery_junk_accessory_legendary()
		{
			static const auto pool = list_recipes([](const game::recipe_t& recipe)
			{
				if (recipe.production == nullptr || recipe.production->type != PRD_TYPE_Accessory || recipe.production->rarity != 4 || !recipe.junk)
				{
					return false;
				}

				const auto& accessory = recipe.production->accessory;
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

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Production>
		game::item_t lottery_production_single()
		{
			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
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

			game::item_t item{};
			item.category = game::ITEM_CATEGORY_RECIPE;
			item.num = 1;

			const auto idx = rand_index(pool);
			item.code = pool[idx];

			return item;
		}

		template <std::uint32_t Rarity>
		game::item_t lottery_material_w_items()
		{
			static const auto pool = []()
			{
				static const auto resources = list_resources(Rarity, [](const game::resource_t& resource)
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

					return set.contains(resource.material) && resource.type == PRD_TYPE_Material;
				});

				static const auto items = list_stackable_items(Rarity, [](const game::production_t& item)
				{
					static std::unordered_set<std::uint32_t> set =
					{
						PRD_TYPE_Ammo,
						PRD_TYPE_Cure,
						PRD_TYPE_Gadget,
						PRD_TYPE_Material,
					};

					return set.contains(item.type);
				});

				std::vector<game::item_t> res;

				for (const auto& code : resources)
				{
					game::item_t item{};
					item.category = game::ITEM_CATEGORY_RESOURCE;
					item.code = code;
					res.emplace_back(item);
				}

				for (const auto& code : items)
				{
					game::item_t item{};
					item.category = game::ITEM_CATEGORY_PRODUCTION;
					item.code = code;
					res.emplace_back(item);
				}

				return res;
			}();

			const auto idx = utils::random::index(pool);
			game::item_t item = pool[idx];
			item.num = utils::random::get_integer(1u, 50u);

			return item;
		}
	}

	void register_battle_pack_lottery(const std::uint32_t type, const lottery_t& lottery)
	{
		lottery();
		battle_pack_lotteries.insert(std::make_pair(type, lottery));
	}

	void register_reward_lottery(const std::uint32_t type, const std::uint8_t rarity, const lottery_t& lottery)
	{
		if (rarity > ui_reward_lotteries.size())
		{
			throw std::runtime_error("[lottery] invalid rarity");
		}

		lottery();
		ui_reward_lotteries[rarity].insert(std::make_pair(type, lottery));
	}

	void initialize_lottery()
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
		register_battle_pack_lottery(BP_Junk_Weapon_Legendary_singleQuest, lottery_production_single<RCP_EQP_WP_sg01_sw1_TypeA>);
		register_battle_pack_lottery(BP_Boss01Reward_Weapon, lottery_production_single<RCP_EQP_WP_sr03_TypeA>);
		register_battle_pack_lottery(BP_Boss01Reward_Accessory, lottery_production_single<RCP_ACC_Body_15_TypeA>);
		register_battle_pack_lottery(BP_Boss01Reward_Material, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss02Reward_WeaponEnhance, lottery_weapon_enhance);
		register_battle_pack_lottery(BP_Boss02Reward_AccessoryEnhance, lottery_weapon_enhance);
		register_battle_pack_lottery(BP_Boss02Reward_Material, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss03Reward_01, lottery_production_single<RCP_ACC_Body_15_TypeA>);
		register_battle_pack_lottery(BP_Boss03Reward_02, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss03Reward_03, lottery_material<2>);
		register_battle_pack_lottery(BP_Boss04Reward_01, lottery_material<4>);
		register_battle_pack_lottery(BP_Boss04Reward_02, lottery_material<3>);
		register_battle_pack_lottery(BP_Boss04Reward_03, lottery_material<2>);
		register_battle_pack_lottery(BP_KubanEnergy_100K, lottery_energy<100'000>);
		register_battle_pack_lottery(BP_Medical_Kit, lottery_medical);

		// 0x901FA9E7 => material
		register_reward_lottery(ui_reward_type_material, 1, lottery_material_w_items<1>);
		register_reward_lottery(ui_reward_type_material, 2, lottery_material_w_items<2>);
		register_reward_lottery(ui_reward_type_material, 3, lottery_material_w_items<3>);
		register_reward_lottery(ui_reward_type_material, 4, lottery_material_w_items<4>);
		register_reward_lottery(ui_reward_type_material, 5, lottery_material_w_items<5>);

		// 0x5650D1B1 => junk weapon
		register_reward_lottery(ui_reward_type_junk_weapon, 1, lottery_junk_weapon<1>);
		register_reward_lottery(ui_reward_type_junk_weapon, 2, lottery_junk_weapon<2>);
		register_reward_lottery(ui_reward_type_junk_weapon, 3, lottery_junk_weapon<3>);
		register_reward_lottery(ui_reward_type_junk_weapon, 4, lottery_junk_weapon<4>);
		register_reward_lottery(ui_reward_type_junk_weapon, 5, lottery_junk_weapon<5>);

		// 0x5650D1B1 => junk gear
		register_reward_lottery(ui_reward_type_junk_accessory, 1, lottery_junk_accessory<1>);
		register_reward_lottery(ui_reward_type_junk_accessory, 2, lottery_junk_accessory<2>);
		register_reward_lottery(ui_reward_type_junk_accessory, 3, lottery_junk_accessory<3>);
		register_reward_lottery(ui_reward_type_junk_accessory, 4, lottery_junk_accessory<4>);
		register_reward_lottery(ui_reward_type_junk_accessory, 5, lottery_junk_accessory<5>);

		// 0x6085F29A => kuban energy
		register_reward_lottery(ui_reward_type_energy, 0, lottery_energy<10000>);
	}
}
	