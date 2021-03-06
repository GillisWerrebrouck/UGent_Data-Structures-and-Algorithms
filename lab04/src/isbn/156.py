def isISBN(code):
    if not (
        isinstance(code, str) and
        len(code) == 13 and
        ( code.startswith('978') or code.startswith('979') ) and
        code.isdigit()
        ):
        return 0

    controle=0
    for i in range(12):
            if i%2:
                controle += 3* int(code[i])
            else:
                controle += int(code[i])
    cc = controle % 10
    cc = (10 - cc) % 10
    return cc == int(code[-1])

def overzicht(codes):
    groepen = {}
    for i in range(11):
        groepen[i] = 0
    for code in codes:
        if not isISBN(code):
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
