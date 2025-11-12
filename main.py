import pandas as pd
from fp_tree import FP_Node, FP_Tree

def load_transactions(path):
    df = pd.read_excel(path)
    transactions = []

    for index, row in df.iterrows():
        # Get the string of items from the 'items' column
        items_str = str(row['items']).strip()

        # Split by commas and clean extra spaces
        items = [item.strip() for item in items_str.split(',') if item.strip()]

        # Add this transaction to the list
        transactions.append(items)

    print("Transactions loaded successfully.\n")
    return transactions

def print_transactions(transactions):
    for transaction in transactions:
        print(transaction)
    print("")

def print_items_support(items_support):
    for item, count in sorted(items_support.items(), key=lambda x: (-x[1], x[0])):
        print(f"{item}: {count}")
    print("")


def main():
    transactions = load_transactions("Horizontal_Format.xlsx")

    print("Initial transactions before pre-processing:")
    print_transactions(transactions)

    fp_tree = FP_Tree(min_support=3, min_confidence=0.8)
    items_support = fp_tree.build_header_table(transactions)

    print("Items support after removeing infrequnet items and sorting:")
    print_items_support(items_support)

    print("Transactions after removing infrequent items and sorting:")
    print_transactions(transactions)

    fp_tree.build_fp_tree(transactions)
    fp_tree.print_tree()


main()
