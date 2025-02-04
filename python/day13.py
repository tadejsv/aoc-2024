from dataclasses import dataclass


@dataclass
class Machine:
    button_a: tuple[int, int]
    button_b: tuple[int, int]
    prize: tuple[int, int]


COST_A = 3
COST_B = 1


# Returns x (multitplier of a), y (multipler of b), gcd
def extended_euclidean(a: int, b: int) -> tuple[int, int, int]:
    if b == 0:
        return 1, 0, a

    x_old, y_old, gcd_ret = extended_euclidean(b, a % b)
    x = y_old
    y = x_old - y_old * (a // b)
    return x, y, gcd_ret


def gcd(a: int, b: int) -> int:
    if b == 0:
        return a
    return gcd(b, a % b)


def solve_machine(machine: Machine) -> int:
    det = (
        machine.button_b[1] * machine.button_a[0]
        - machine.button_b[0] * machine.button_a[1]
    )

    if det == 0:
        return 0

    p_part = (
        machine.prize[1] * machine.button_a[0] - machine.prize[0] * machine.button_a[1]
    )
    b = p_part // det

    a = (machine.prize[0] - b * machine.button_b[0]) // machine.button_a[0]

    if a < 0 or b < 0:
        return 0

    p0 = machine.button_a[0] * a + machine.button_b[0] * b
    p1 = machine.button_a[1] * a + machine.button_b[1] * b

    if p0 == machine.prize[0] and p1 == machine.prize[1]:
        return a * COST_A + b * COST_B

    return 0


def solve_machine_extra(machine: Machine) -> int:
    det = (
        machine.button_b[1] * machine.button_a[0]
        - machine.button_b[0] * machine.button_a[1]
    )

    b: int
    if det == 0:
        # Test for inconsistency
        if (
            machine.button_a[0] // machine.button_a[1]
            != machine.prize[0] // machine.prize[1]
        ):
            return 0

        # Test for lack of integer solutions
        if machine.prize[0] % gcd(machine.button_a[0], machine.button_b[0]) != 0:
            return 0

        a_prime = machine.button_a[0] // gcd(machine.button_a[0], machine.button_b[0])
        b_inv, _, _ = extended_euclidean(machine.button_b[0], a_prime)
        b_mod = (machine.prize[0] * b_inv) % a_prime
        if b_mod < 0:
            b_mod += a_prime

        # Using cross-multiplication to avoid casting to float to compute
        # ca / cb > a[0] / b[0]
        if COST_A * machine.button_b[0] > machine.button_a[0] * COST_B:
            b_max = machine.prize[0] // machine.button_b[0]
            b = a_prime * (b_max // a_prime) + b_mod
            if b > b_max:
                b -= a_prime
        else:
            b = b_mod
    else:
        p_part = (
            machine.prize[1] * machine.button_a[0]
            - machine.prize[0] * machine.button_a[1]
        )
        b = p_part // det

    a = (machine.prize[0] - b * machine.button_b[0]) // machine.button_a[0]

    if a < 0 or b < 0:
        return 0

    p0 = machine.button_a[0] * a + machine.button_b[0] * b
    p1 = machine.button_a[1] * a + machine.button_b[1] * b

    if p0 == machine.prize[0] and p1 == machine.prize[1]:
        return a * COST_A + b * COST_B

    return 0


def day13a() -> None:
    machines: list[Machine] = []

    machine = Machine((0, 0), (0, 0), (0, 0))
    with open("../inputs/day13.txt", "r") as file:
        for line in file:
            if len(line.strip()) == 0:
                machines.append(machine)
                machine = Machine((0, 0), (0, 0), (0, 0))
                continue

            parts = line.strip().split(": ")
            if parts[0] == "Prize":
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("=")[1]) for pc in parts_coords)
                machine.prize = coords
            else:
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("+")[1]) for pc in parts_coords)

                if parts[0] == "Button A":
                    machine.button_a = coords
                else:
                    machine.button_b = coords

    machines.append(machine)

    total_tokens = 0
    for machine in machines:
        total_tokens += solve_machine(machine)

    print(total_tokens)


def day13b() -> None:
    machines: list[Machine] = []

    machine = Machine((0, 0), (0, 0), (0, 0))
    with open("../inputs/day13.txt", "r") as file:
        for line in file:
            if len(line.strip()) == 0:
                machines.append(machine)
                machine = Machine((0, 0), (0, 0), (0, 0))
                continue

            parts = line.strip().split(": ")
            if parts[0] == "Prize":
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("=")[1]) for pc in parts_coords)
                add_const = 10000000000000
                machine.prize = (coords[0] + add_const, coords[1] + add_const)
            else:
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("+")[1]) for pc in parts_coords)

                if parts[0] == "Button A":
                    machine.button_a = coords
                else:
                    machine.button_b = coords

    machines.append(machine)

    total_tokens = 0
    for machine in machines:
        total_tokens += solve_machine(machine)

    print(total_tokens)


def day13extra() -> None:
    machines: list[Machine] = []

    machine = Machine((0, 0), (0, 0), (0, 0))
    with open("../inputs/day13_extra.txt", "r") as file:
        for line in file:
            if len(line.strip()) == 0:
                machines.append(machine)
                machine = Machine((0, 0), (0, 0), (0, 0))
                continue

            parts = line.strip().split(": ")
            if parts[0] == "Prize":
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("=")[1]) for pc in parts_coords)
                machine.prize = (coords[0], coords[1])
            else:
                parts_coords = parts[1].split(", ")
                coords = tuple(int(pc.split("+")[1]) for pc in parts_coords)

                if parts[0] == "Button A":
                    machine.button_a = coords
                else:
                    machine.button_b = coords

    machines.append(machine)

    total_tokens = 0
    for machine in machines:
        total_tokens += solve_machine_extra(machine)

    print(total_tokens)


day13extra()
