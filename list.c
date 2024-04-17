struct node {
	int key;
	struct node *prev;
	struct node *next;
};

struct list {
	struct node *head;
};

int list_search(struct list *L, int k) {
	struct node *x = L->head;

	while (x != NULL && x->key != k) {
		x = x->next;
	}

	return x;
}

void list_insert(struct list *L, struct node *x) {
	x->next = L->head;

	if (L.head != NULL) {
		L->head->prev = x;
	}

	L->head = x;
	x->prev = NULL;
}

void list_delete(struct list *L, struct node *x) {
	if (x->prev != NULL) {
		x->prev->next = x->next;
	} else {
		L->head = x->next;
	}
	if (x->next != NULL) {
		x->next->prev = x->prev;
	}
}
