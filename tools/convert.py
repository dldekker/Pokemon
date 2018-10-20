import json

file = open('move.json', 'r')
text = file.read()

moves = json.loads(text)

l = []
for name in moves:
    move = moves[name]

    if move['index_number'] >= 560:
        continue
    
    index_number = move['index_number']
    pp = move['pp']
    max_pp = move['max_pp']
    power = move['power']
    accuracy = move['accuracy']

    if move['category'] == 'special':
        category = 'special'
    elif move['category'] == 'physical':
        category = 'physical'
    elif move['category'] == 'status':
        category = 'status'

    priority = move['priority']
    target = move['target']
    critical_hit = move['critical_hit']
    makes_contact = move['makes_contact']

    affected_by_protect = move['affected_by_protect']
    affected_by_magic_coat = move['affected_by_magic_coat']
    affected_by_snatch = move['affected_by_snatch']
    affected_by_mirror_move = move['affected_by_mirror_move']
    affected_by_kings_rock = move['affected_by_kings_rock']

    name = move['names']['en']
    move_type = move['type']
    pokedex_entry = move['pokedex_entries']['Black']['en']

    l.append("{"+"{:>3}, {:>2}, {:>2}, {:>3}, {:>3}, {:>8}, {:>2}, {:>28}, {},{},{},{},{},{},{}, {:>16}, {:>8}, {}".format(
        int(index_number),
        int(pp),
        int(max_pp),
        int(power),
        int(accuracy),
        category.upper(),
        int(priority),
        target.upper(),
        int(critical_hit),
        
        int(makes_contact),
        int(affected_by_protect),
        int(affected_by_magic_coat),
        int(affected_by_snatch),
        int(affected_by_mirror_move),
        int(affected_by_kings_rock),

        '"' + name + '"',
        move_type.upper(),
        '"' + pokedex_entry + '"'
    )+"},")

l = sorted(l)
for a in l:
    print(a)
