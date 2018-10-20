import json

file = open('pokemon.json', 'r')
text = file.read()

pokemons = json.loads(text)

l = []
for name in pokemons:
    p = pokemons[name]
    s = "{"

    # National ID
    index = p["national_id"];
    if index > 649:
        continue;
    s += "{:>3},".format(index)

    # Name
    name = p["names"]['en']
    s += "{:>12},".format("\"" + name + "\"")

    # Types
    types = p["types"]
    if (len(types) == 2):
        s += "{}{:>8}, {:>8}{},".format("{", types[0].upper(), types[1].upper(), "}")
    elif (len(types) == 1):
        s += "{}{:>8}, {:>8}{},".format("{", types[0].upper(), "NONE", "}")

    # Abilities
    abilities = p["abilities"]
    a1 = None
    a2 = None
    ah = None
    for a in abilities:
        if "hidden" in a:
            ah = a['name']
            continue
        if a1 == None:
            a1 = a['name']
    s += "{}{:>16},{:>16}{}".format("{", "\""+a1+"\"", "\"NoAbility\"", "},")
    if ah is not None:
        s += "{:>17}".format("\"" + ah + "\", ")
    else:
        s += "{:>17}".format("\"" + "NoAbility" + "\", ")

    # Gender Ratios
    gr = p["gender_ratios"]
    if gr == None:
        s += "{  0.0,  0.0},";
    elif "male" not in gr:
        s += "{" + "  0.0," + "{:.1f}".format(gr["female"]) + "},"
    elif "female" not in gr:
        s += "{" + "{:.1f}".format(gr["male"]) + ",  0.0" + "},"
    else:
        s += "{" + "{:5.1f}".format(gr["male"]) + ",{:5.1f}".format(gr["female"]) + "},"

    # Catch Rate
    s += "{:>3},".format(p["catch_rate"]) 
    
    # Egg Group
    egg_groups = p["egg_groups"]
    if (egg_groups[0] == "Field"): s += "{" + "{:>22}".format("EGG_FIELD_GROUP")
    elif (egg_groups[0] == "Dragon"): s += "{" + "{:>22}".format("EGG_DRAGON_GROUP")
    elif (egg_groups[0] == "Flying"): s += "{" + "{:>22}".format("EGG_FLYING_GROUP")
    elif (egg_groups[0] == "Undiscovered"): s += "{" + "{:>22}".format("EGG_UNDISCOVERED_GROUP")
    elif (egg_groups[0] == "Bug"): s += "{" + "{:>22}".format("EGG_BUG_GROUP")
    elif (egg_groups[0] == "Mineral"): s += "{" + "{:>22}".format("EGG_MINERAL_GROUP")
    elif (egg_groups[0] == "Water 1"): s += "{" + "{:>22}".format("EGG_WATER_1_GROUP")
    elif (egg_groups[0] == "Water 2"): s += "{" + "{:>22}".format("EGG_WATER_2_GROUP")
    elif (egg_groups[0] == "Water 3"): s += "{" + "{:>22}".format("EGG_WATER_3_GROUP")
    elif (egg_groups[0] == "Amorphous"): s += "{" + "{:>22}".format("EGG_AMORPHOUS_GROUP")
    elif (egg_groups[0] == "Fairy"): s += "{" + "{:>22}".format("EGG_FAIRY_GROUP")
    elif (egg_groups[0] == "Monster"): s += "{" + "{:>22}".format("EGG_MONSTER_GROUP")
    elif (egg_groups[0] == "Grass"): s += "{" + "{:>22}".format("EGG_GRASS_GROUP")
    elif (egg_groups[0] == "Human-Like"): s += "{" + "{:>22}".format("EGG_HUMAN_LIKE_GROUP")
    elif (egg_groups[0] == "Ditto"): s += "{" + "{:>22}".format("EGG_DITTO_GROUP")
    if (len(egg_groups) == 2):
        if (egg_groups[1] == "Field"): s += "," + "{:>22}".format("EGG_FIELD_GROUP")
        elif (egg_groups[1] == "Dragon"): s += "," + "{:>22}".format("EGG_DRAGON_GROUP")
        elif (egg_groups[1] == "Flying"): s += "," + "{:>22}".format("EGG_FLYING_GROUP")
        elif (egg_groups[1] == "Undiscovered"): s += "," + "{:>22}".format("EGG_UNDISCOVERED_GROUP")
        elif (egg_groups[1] == "Bug"): s += "," + "{:>22}".format("EGG_BUG_GROUP")
        elif (egg_groups[1] == "Mineral"): s += "," + "{:>22}".format("EGG_MINERAL_GROUP")
        elif (egg_groups[1] == "Water 1"): s += "," + "{:>22}".format("EGG_WATER_1_GROUP")
        elif (egg_groups[1] == "Water 2"): s += "," + "{:>22}".format("EGG_WATER_2_GROUP")
        elif (egg_groups[1] == "Water 3"): s += "," + "{:>22}".format("EGG_WATER_3_GROUP")
        elif (egg_groups[1] == "Amorphous"): s += "," + "{:>22}".format("EGG_AMORPHOUS_GROUP")
        elif (egg_groups[1] == "Fairy"): s += "," + "{:>22}".format("EGG_FAIRY_GROUP")
        elif (egg_groups[1] == "Monster"): s += "," + "{:>22}".format("EGG_MONSTER_GROUP")
        elif (egg_groups[1] == "Grass"): s += "," + "{:>22}".format("EGG_GRASS_GROUP")
        elif (egg_groups[1] == "Human-Like"): s += "," + "{:>22}".format("EGG_HUMAN_LIKE_GROUP")
        elif (egg_groups[1] == "Ditto"): s += "," + "{:>22}".format("EGG_DITTO_GROUP")
    else:
        s += "," + "{:>22}".format("EGG_NONE_GROUP")
    s += "},"

    # Hatching Time
    hatch_time = p["hatch_time"]
    s += "{" + "{:>5}".format(hatch_time[0]) + "," + "{:>5}".format(hatch_time[1]) + "},"

    # Height, Weight
    height = float(p["height_eu"][:-2])
    weight = float(p["weight_eu"][:-3])
    s += "" + "{:4.1f}".format(height) + "," + "{:5.1f}".format(weight) + ","

    # Base EXP yield
    s += "{:>3},".format(p["base_exp_yield"])

    # Leveling Rate
    rate = p["leveling_rate"]
    if rate == "Erratic": r = "EXP_ERRATIC"
    elif rate == "Fast": r = "EXP_FAST"
    elif rate == "Medium Fast": r = "EXP_MEDIUM_FAST"
    elif rate == "Medium Slow": r = "EXP_MEDIUM_SLOW"
    elif rate == "Slow": r = "EXP_SLOW"
    elif rate == "Fluctuating": r = "EXP_FLUCTUATING"
    s += "{:>15},".format(r)

    # EV Yield
    ev = p["ev_yield"]
    s += "{" + str(ev["hp"]) + "," + str(ev["atk"]) + "," + str(ev["def"]) + "," + str(ev["sp_atk"]) + "," + str(ev["sp_def"]) + "," + str(ev["speed"]) + "},"

    # Base Friendship
    base_friendship = p["base_friendship"]
    s += "{:>3},".format(base_friendship)

    # Base Stats
    stats = p["base_stats"]
    s += "{" + "{:>3}".format(stats["hp"]) + ","
    s += "{:>3}".format(stats["atk"]) + ","
    s += "{:>3}".format(stats["def"]) + ","
    s += "{:>3}".format(stats["sp_atk"]) + ","
    s += "{:>3}".format(stats["sp_def"]) + ","
    s += "{:>3}".format(stats["speed"]) + "},"

    # Pokedex Discription
    s += "{:>128}".format("\"" + p["pokedex_entries"]["Black"]["en"] + "\",")

    # Evolutions
    evolutions = p["evolutions"] 
    m = []
    for e in evolutions:
        es = ""
        if len(e) != 0:
            if len(e) == 2 and "level" in e:
                es += "{{EVOLUTION_LEVEL,{:>2},{{}},0}}".format(e["level"])
            elif len(e) == 2 and "move_learned" in e:
                es += "{{EVOLUTION_MOVE,0,{},0}}".format("\"" + e["move_learned"] + "\"")
            elif len(e) == 2 and "happiness" in e:
                es += "{EVOLUTION_HAPPINESS,0,{},0}"
            elif len(e) == 2 and "item" in e:
                es += "{{EVOLUTION_ITEM,0,{},0}}".format("\"" + e["item"] + "\"")
            elif len(e) == 2 and "trade" in e:
                es += "{EVOLUTION_TRADE,0,{},0}"
            elif len(e) == 3 and "trade" in e and "hold_item" in e:
                es += "{{EVOLUTION_TRADE_ITEM,0,{},0}}".format("\"" + e["hold_item"] + "\"")

            elif len(e) == 3 and "happiness" in e and "conditions" in e:
                if e['conditions'] == ['Daytime']:
                    es += "{EVOLUTION_HAPPINESS,0,{},DAYTIME}"
                elif e['conditions'] == ['Nighttime']:
                    es += "{EVOLUTION_HAPPINESS,0,{},NIGHTTIME}"

            elif len(e) == 3 and "level_up" in e and "conditions" in e:
                if e['conditions'] == ['Near a Mossy Rock']:
                    es += "{EVOLUTION_MOSSY_ROCK,0,{},0}"
                elif e['conditions'] == ['Near an Icy Rock']:
                    es += "{EVOLUTION_ICY_ROCK,0,{},0}"
                elif e['conditions'] == ['In a Magnetic Field area']:
                    es += "{EVOLUTION_MAGNETIC_FIELD,0,{},0}"
                elif e['conditions'] == ['With Remoraid in party']:
                    es += "{EVOLUTION_MAGNETIC_FIELD,0,{},0}"

            elif len(e) == 3 and "item" in e and "conditions" in e:
                if e['conditions'] == ['Male']:
                    es += "{{EVOLUTION_ITEM_MALE,0,{},0}}".format("\""+e["item"]+"\"")
                elif e['conditions'] == ['Female']:
                    es += "{{EVOLUTION_ITEM_FEMALE,0,{},0}}".format("\""+e["item"]+"\"")

            elif len(e) == 3 and "hold_item" in e and "conditions" in e:
                if e['conditions'] == ['Nighttime']:
                    es += "{{EVOLUTION_ITEM,0,{},NIGHTTIME}}".format("\"" + e["hold_item"] + "\"")
                elif e['conditions'] == ['Daytime']:
                    es += "{{EVOLUTION_ITEM,0,{},DAYTIME}}".format("\"" + e["hold_item"] + "\"")
                elif e['conditions'] == ['Morning']:
                    es += "{{EVOLUTION_ITEM,0,{},MORNING}}".format("\"" + e["hold_item"] + "\"")

            elif len(e) == 2 and "conditions" in e:
                if e["conditions"] == ['Beauty']:
                    es += "{EVOLUTION_BEAUTY,0,{},0}"

            elif "level" in e:
                conditions = e['conditions']
                if 'Outside' in conditions and 'Female' in conditions:
                    es += "{EVOLUTION_LEVEL_FEMALE_OUTSIDE,0,{},0}"
                elif 'In Caves' in conditions and 'Female' in conditions:
                    es += "{EVOLUTION_LEVEL_FEMALE_CAVES,0,{},0}"
                elif 'In Buildings' in conditions and 'Female' in conditions:
                    es += "{EVOLUTION_LEVEL_FEMALE_INSIDE,0,{},0}"
                elif len(e) == 3 and 'Male' in conditions:
                    es += "{EVOLUTION_LEVEL_MALE,0,{},0}"
                elif len(e) == 3 and 'Female' in conditions:
                    es += "{EVOLUTION_LEVEL_FEMALE,0,{},0}"
                elif 'Random' in conditions:
                    es += "{EVOLUTION_LEVEL_RANDOM,0,{},0}"
                elif 'With empty spot in party' in conditions:
                    es += "{EVOLUTION_LEVEL_EMPTY_SPOT,0,{},0}"
                elif 'Attack > Defense' in conditions:
                    es += "{EVOLUTION_LEVEL_ATK_LARGER_DEF,0,{},0}"
                elif 'Defense > Attack' in conditions:
                    es += "{EVOLUTION_LEVEL_ATK_SMALLER_DEF,0,{},0}"
                elif 'Attack = Defense' in conditions:
                    es += "{EVOLUTION_LEVEL_ATK_EQUAL_DEF,0,{},0}"
        if es != "":
            m.append(es)
    s += "{" + ",".join(m) + "},"

    # Moves
    movesets = p['move_learnsets']
    m = []
    for moveset in movesets:
        if "Black" in moveset["games"]:
            for move in moveset["learnset"]:
                ms = ""
                if "tm" in move:
                    ms += "{{{},0,-1,1}}".format("\"" + move['move'] + "\"")
                elif "egg_move" in move:
                    ms += "{{{},1,-1,0}}".format("\"" + move['move'] + "\"")
                elif "level" in move:
                    ms += "{{{},0,{},0}}".format("\"" + move['move'] + "\"", move['level'])
                if m != "":
                    m.append(ms)
    s += "{" + ",".join(m) + "}"

    s += "},"

    l.append(s)


l = sorted(l)
for i in l:
    print(i)
