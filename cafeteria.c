#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10

// -------- MENU --------
struct Menu {
    int id;
    char name[50];
    float price;
};

struct Menu menu[] = {
    {1, "Burger", 150},
    {2, "Pizza", 200},
    {3, "Coffee", 80},
    {4, "Noodles", 60},
    {5, "Cholo Bhature", 250},
    {6, "Cutlet", 120}
};

int menuSize = 6;

// -------- CART --------
struct CartItem {
    int id;
    int qty;
    float total;
};

struct CartItem cart[MAX];
int cartSize = 0;

// -------- STACK (UNDO) --------
struct CartItem stack[MAX];
int top = -1;

// -------- QUEUE (ORDER PROCESS) --------
struct CartItem queue[MAX];
int front = -1, rear = -1;

// -------- FUNCTIONS --------
float getPrice(int id) {
    for (int i = 0; i < menuSize; i++) {
        if (menu[i].id == id)
            return menu[i].price;
    }
    return 0;
}

char* getName(int id) {
    for (int i = 0; i < menuSize; i++) {
        if (menu[i].id == id)
            return menu[i].name;
    }
    return "";
}

// -------- DISPLAY MENU --------
void showMenu() {
    printf("\n------ MENU ------\n");
    for (int i = 0; i < menuSize; i++) {
        printf("%d. %s - Rs %.2f\n", menu[i].id, menu[i].name, menu[i].price);
    }
}

// -------- ADD TO CART --------
void addItem(int id, int qty) {
    float price = getPrice(id);
    if (price == 0) {
        printf("Invalid item!\n");
        return;
    }

    struct CartItem item;
    item.id = id;
    item.qty = qty;
    item.total = price * qty;

    cart[cartSize++] = item;

    // push to stack
    stack[++top] = item;

    printf("Added %s x%d\n", getName(id), qty);
}

// -------- UNDO --------
void undo() {
    if (top == -1) {
        printf("Nothing to undo!\n");
        return;
    }

    printf("Removed %s\n", getName(stack[top].id));
    cartSize--;
    top--;
}

// -------- BILL --------
void showBill() {
    float subtotal = 0;

    printf("\n------ CART ------\n");
    for (int i = 0; i < cartSize; i++) {
        printf("%s x%d = Rs %.2f\n",
            getName(cart[i].id),
            cart[i].qty,
            cart[i].total);

        subtotal += cart[i].total;
    }

    float discount = subtotal * 0.10;
    float tax = subtotal * 0.05;
    float total = subtotal - discount + tax;

    printf("\nSubtotal: Rs %.2f\n", subtotal);
    printf("Discount (10%%): Rs %.2f\n", discount);
    printf("Tax (5%%): Rs %.2f\n", tax);
    printf("TOTAL: Rs %.2f\n", total);
}

// -------- QUEUE --------
void placeOrder() {
    if (cartSize == 0) {
        printf("Cart empty!\n");
        return;
    }

    for (int i = 0; i < cartSize; i++) {
        if (rear == MAX - 1) {
            printf("Queue full!\n");
            return;
        }
        if (front == -1) front = 0;

        queue[++rear] = cart[i];
    }

    printf("Order placed successfully!\n");

    // clear cart
    cartSize = 0;
    top = -1;
}

// -------- PROCESS ORDER --------
void processOrder() {
    if (front == -1 || front > rear) {
        printf("No orders!\n");
        return;
    }

    printf("Processing %s\n", getName(queue[front].id));
    front++;
}

// -------- MAIN --------
int main() {
    int choice, id, qty;

    while (1) {
        printf("\n1.Menu  2.Add  3.Undo  4.Bill  5.Place Order  6.Process  7.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            showMenu();
            break;

        case 2:
            showMenu();
            printf("Enter item id: ");
            scanf("%d", &id);
            printf("Enter qty: ");
            scanf("%d", &qty);
            addItem(id, qty);
            break;

        case 3:
            undo();
            break;

        case 4:
            showBill();
            break;

        case 5:
            placeOrder();
            break;

        case 6:
            processOrder();
            break;

        case 7:
            exit(0);

        default:
            printf("Invalid!\n");
        }
    }
}