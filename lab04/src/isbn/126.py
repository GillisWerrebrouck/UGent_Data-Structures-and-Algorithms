def isISBN13(code):
    if not (
        isinstance(code, str) and
        len(code) == 13 and
        code.isdigit()
    ):
        return False
    if code[:3] not in {'978', '979'}:
        return False

    controle = sum((3 if i % 2 else 1) * int(code[i]) for i in range(12))
    controle = (10 - controle % 10) % 10
    return controle == int(code[-1])


def overzicht(codes):
    groepen = {}
    for i in range(11):
        groepen[i] = 0
    for code in codes:
        if not isISBN13(code):
            groepen[10] += 1
        else:
            groepen[int(code[3])] += 1

    print('Engelstalige landen: {}'.format(groepen[0] + groepen[1]))
    print('Franstalige landen: {}'.format(groepen[2]))
    print('Duitstalige landen: {}'.format(groepen[3]))
    print('Japan: {}'.format(groepen[4]))
    print('Russischtalige landen: {}'.format(groepen[5]))
    print('China: {}'.format(groepen[7]))
    print('Overige landen: {}'.format(groepen[6] + groepen[8] + groepen[9]))
    print('Fouten: {}'.format(groepen[10]))