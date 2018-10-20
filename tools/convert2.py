import json

file = open('abilities.json', 'r')
text = file.read()

pokemons = json.loads(text)

l = []
for pokemon in pokemons:
    l.append("{"
        + "{:>3}".format(pokemons[pokemon]["index_number"]) + ", "
        + "{:>18}".format("\"" + pokemons[pokemon]["names"]["en"] + "\"") + ", "
        + "{}".format("\"" + pokemons[pokemon]["descriptions"]["en"] + "\"")
        + "},")

l = sorted(l)
for i in l:
    print(i)
