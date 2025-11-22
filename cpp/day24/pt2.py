from collections import defaultdict

file_name = "../inputs/day24.txt"

with open(file_name) as f:
    lines = [l.strip() for l in f.readlines()]

relations = []
for i, l in enumerate(lines):
    if len(l) == 0:
        relations = lines[i + 1 :]
        break

in_rels = {}
out_rels = defaultdict(list)
for rel in relations:
    parts = rel.split(" ")
    in_rels[parts[-1]] = {"operator": parts[1], "operands": [parts[0], parts[2]]}
    out_rels[parts[0]].append(parts[-1])
    out_rels[parts[2]].append(parts[-1])

# Assume z00 is OK
last_c = [x for x in out_rels["x00"] if x != "z00"][0]
if last_c not in out_rels["y00"]:
    raise ValueError("Wrong c00")

i = 1
while True:
    num = f"{i:02d}"
    if f"x{num}" not in out_rels:
        break
    mix = set(out_rels[f"x{num}"]).intersection(out_rels[f"y{num}"])
    mix = list(mix)

    z_prime = mix[0]
    if in_rels[mix[0]]["operator"] != "XOR":
        z_prime = mix[1]

    c_prime = mix[0] if z_prime == mix[1] else mix[1]

    if in_rels[z_prime]["operator"] != "XOR":
        raise ValueError(f"Wrong operator for z_prime {z_prime} at {num}")

    if in_rels[c_prime]["operator"] != "AND":
        raise ValueError(f"Wrong operator for c_prime {c_prime} at {num}")

    if set(in_rels[z_prime]["operands"]) != {f"x{num}", f"y{num}"}:
        raise ValueError(f"Wrong operands for z_prime {z_prime} at {num}")

    if set(in_rels[c_prime]["operands"]) != {f"x{num}", f"y{num}"}:
        raise ValueError(f"Wrong operands for c_prime {c_prime} at {num}")

    print(f"{num}: {last_c=}, {c_prime=}, {z_prime=}")

    current_z = in_rels[f"z{num}"]
    if current_z["operator"] != "XOR":
        raise ValueError(f"Wrong operator for z{num}")

    if set(current_z["operands"]) != {last_c, z_prime}:
        raise ValueError(f"Wrong operands for z{num}")

    c_prime2 = set(out_rels[z_prime]).intersection(out_rels[last_c]) - {f"z{num}"}
    if len(c_prime2) != 1:
        raise ValueError(f"Wrong length of c_prime2 candidates at {num}: {c_prime2}")

    c_prime2 = list(c_prime2)[0]
    if in_rels[c_prime2]["operator"] != "AND":
        raise ValueError(f"Wrong operator for c_prime2 {c_prime2} at {num}")

    print(f"{num}: {c_prime2=}")

    new_c = set(out_rels[c_prime2]).intersection(out_rels[c_prime])
    if len(new_c) != 1:
        raise ValueError(f"Wrong length of new_c candidates at {num}: {new_c}")
    new_c = list(new_c)[0]
    if in_rels[new_c]["operator"] != "OR":
        raise ValueError(f"Wrong operator for new_c {new_c} at {num}")

    last_c = new_c

    i += 1


# Errors found:
# y16 AND x16 -> z16, vmr XOR bnc -> hmk | switch z16 and hmk
# pns AND tsc -> z20, tsc XOR pns -> fhp | switch z20 and fhp
# rvf XOR dkb -> z27 - actually need to switch x27 AND y27 -> rvf, y27 XOR x27 -> tpc | switch tpc and rvf
# wkw OR jgr -> z33, smf XOR rfd -> fcd | switch z33 and fcd

# All gates
# z16,hmk,z20,fhp,tpc,rvf,z33,fcd
# sorted: fcd,fhp,hmk,rvf,tpc,z16,z20,z33
