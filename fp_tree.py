class FP_Node:
    def __init__(self, item):
        self._item = item
        self._count = 1
        self._parent = None
        self._children = {}

class FP_Tree:
    def __init__(self, min_support, min_confidence):
        self._root = None
        self._min_support = min_support
        self._min_confidence = min_confidence

    def build_header_table(self, transactions):
        items_support = self._calculate_items_support(transactions)
        self._remove_infrequent_items(items_support, transactions)
        self._sort_items(items_support, transactions)
        return items_support

    def _calculate_items_support(self, transactions):
        items_support = {}

        for transaction in transactions:
            items = set()
            i = 0

            while i < len(transaction):
                item = transaction[i]

                if item not in items:
                    items.add(item)

                    if item in items_support:
                        items_support[item] += 1
                    else:
                        items_support[item] = 1
                    i += 1
                else:
                    transaction.pop(i)

        return items_support

    def _remove_infrequent_items(self, items_support, transactions):
        infrequent_items = [item for item, count in items_support.items()
                            if count < self._min_support]
        
        for item in infrequent_items:
            self._remove_item(item, transactions)
            del items_support[item]

    def _remove_item(self, item_to_remove, transactions):
        for transaction in transactions:
            while item_to_remove in transaction:
                transaction.remove(item_to_remove)

    def _sort_items(self, items_support, transactions):
        for transaction in transactions:
            transaction.sort(key=lambda x: (-items_support[x], x))

    def build_fp_tree(self, transactions):
        self._root = FP_Node("")
        self._root._count = 0

        for transaction in transactions:
            parent = self._root

            for item in transaction:
                parent = self._insert_tree(item, parent)
    
    def _insert_tree(self, item, parent):
        if item not in parent._children:
            new_node = FP_Node(item)
            new_node._parent = parent
            parent._children[item] = new_node
            return new_node
        else:
            parent._children[item]._count += 1
            return parent._children[item]

    def print_tree(self):
        print("FP-Tree Structure:")
        
        for child in self._root._children.values():
            self._print_recursive(child, 1)

    def _print_recursive(self, node, depth):
        if node is None:
            return
        
        print("  " * depth + f"-> {node._item} ({node._count})")

        for child in node._children.values():
            self._print_recursive(child, depth + 1)
