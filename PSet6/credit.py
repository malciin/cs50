def main():
    print("Number: ", end="")
    credit = input()
    print(checkCard(credit))

def checkCard(credit):
    sum = 0
    credit = credit[::-1] #reverse credit card number
    for i in range(0, len(credit)//2):
        value = int(credit[i*2 + 1]) * 2
        if value > 9:
            sum += value // 10 + value % 10
        else:
            sum += value

    for i in range(0, (len(credit)+1)//2):
        sum+= int(credit[i*2])
    
    if sum % 10 != 0:
        return "INVALID"

    if credit[:1] == "4":
        return "VISA"
    if credit[:1] == "3":
        return "AMEX"
    if credit[:1] == "5":
        return "MasterCard"
    return "UNIDENTIFIED"

main()