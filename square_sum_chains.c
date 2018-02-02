#include <stdio.h>
#include <stdlib.h>

typedef struct number_s number_t;

struct number_s {
	int value;
	int used;
	int first_choices_n;
	number_t **choices;
	int next_choices_n;
};

typedef struct {
	int v;
	number_t *last;
}
call_t;

void set_number(number_t *, int);
void eval_number_first(number_t *, number_t **);
int is_valid_sum(int, int);
int compare_numbers_first(const void *, const void *);
int add_call(int, number_t *);
void set_call(call_t *, int, number_t *);
void square_sum_chains(int, number_t *);
void eval_number_next(number_t *);
int compare_numbers_next(const void *, const void *);
void print_error(const char *);

int n, squares_n, *squares, *values, calls_max, calls_n, found;
number_t *numbers;
call_t *calls;

int main(void) {
	int squares_max, first_choices_sum, first_max, i;
	number_t **all_choices, **first_choices;
	if (scanf("%d", &n) != 1 || n < 2) {
		print_error("Invalid order");
		return EXIT_FAILURE;
	}
	squares_max = n*2;
	for (i = 2; i*i < squares_max; i++);
	squares_n = i-2;
	if (squares_n == 0) {
		puts("Not possible");
		return EXIT_SUCCESS;
	}
	squares = malloc(sizeof(int)*(size_t)squares_n);
	if (!squares) {
		print_error("Could not allocate memory for squares");
		return EXIT_FAILURE;
	}
	for (i = 2; i*i < squares_max; i++) {
		squares[i-2] = i*i;
	}
	numbers = malloc(sizeof(number_t)*(size_t)n);
	if (!numbers) {
		print_error("Could not allocate memory for numbers");
		free(squares);
		return EXIT_FAILURE;
	}
	first_choices_sum = 0;
	for (i = 0; i < n; i++) {
		set_number(numbers+i, i+1);
		first_choices_sum += numbers[i].first_choices_n;
	}
	all_choices = malloc(sizeof(number_t *)*(size_t)first_choices_sum);
	if (!all_choices) {
		print_error("Could not allocate memory for all_choices");
		free(numbers);
		free(squares);
		return EXIT_FAILURE;
	}
	first_choices_sum = 0;
	for (i = 0; i < n; i++) {
		eval_number_first(numbers+i, all_choices+first_choices_sum);
		first_choices_sum += numbers[i].first_choices_n;
	}
	first_max = n/2;
	if (n%2 == 1) {
		first_max++;
	}
	first_choices = malloc(sizeof(number_t *)*(size_t)first_max);
	if (!first_choices) {
		print_error("Could not allocate memory for first_choices");
		free(all_choices);
		free(numbers);
		free(squares);
		return EXIT_FAILURE;
	}
	for (i = 0; i < first_max; i++) {
		first_choices[i] = numbers+i;
	}
	qsort(first_choices, (size_t)first_max, sizeof(number_t *), compare_numbers_first);
	values = malloc(sizeof(int)*(size_t)n);
	if (!values) {
		print_error("Could not allocate memory for values");
		free(first_choices);
		free(all_choices);
		free(numbers);
		free(squares);
		return EXIT_FAILURE;
	}
	calls = malloc(sizeof(call_t)*(size_t)n);
	if (!calls) {
		print_error("Could not allocate memory for calls");
		free(values);
		free(first_choices);
		free(all_choices);
		free(numbers);
		free(squares);
		return EXIT_FAILURE;
	}
	calls_max = n;
	calls_n = 0;
	found = 0;
	for (i = first_max; i > 0 && add_call(1, first_choices[i-1]); i--);
	while (calls_n > 0) {
		calls_n--;
		square_sum_chains(calls[calls_n].v, calls[calls_n].last);
	}
	if (found) {
		int j;
		for (j = 0; j < n-1; j++) {
			printf("%d ", values[j]);
		}
		printf("%d\n", values[j]);
	}
	else {
		puts("Not possible");
	}
	fflush(stdout);
	free(calls);
	free(values);
	free(first_choices);
	free(all_choices);
	free(numbers);
	free(squares);
	return EXIT_SUCCESS;
}

void set_number(number_t *number, int value) {
	int i;
	number->value = value;
	number->used = 0;
	number->first_choices_n = 0;
	for (i = 0; i < squares_n; i++) {
		if (is_valid_sum(squares[i], number->value)) {
			number->first_choices_n++;
		}
	}
}

void eval_number_first(number_t *number, number_t **choices) {
	int first_choices_n = 0, i;
	for (i = 0; i < squares_n; i++) {
		if (is_valid_sum(squares[i], number->value)) {
			choices[first_choices_n++] = numbers+squares[i]-number->value-1;
		}
	}
	number->choices = choices;
}

int is_valid_sum(int square, int value) {
	int sum = square-value;
	return sum >= 1 && sum <= n && sum != value;
}

int compare_numbers_first(const void *a, const void *b) {
	number_t *number_a = *(number_t * const *)a, *number_b = *(number_t * const *)b;
	if (number_a->first_choices_n != number_b->first_choices_n) {
		return number_a->first_choices_n-number_b->first_choices_n;
	}
	return number_a->value-number_b->value;
}

int add_call(int v, number_t *last) {
	if (calls_n == calls_max) {
		call_t *calls_tmp = realloc(calls, sizeof(call_t)*(size_t)(calls_max+n));
		if (!calls_tmp) {
			print_error("Could not reallocate memory for calls");
			return 0;
		}
		calls = calls_tmp;
		calls_max += n;
	}
	set_call(calls+calls_n, v, last);
	calls_n++;
	return 1;
}

void set_call(call_t *call, int v, number_t *last) {
	call->v = v;
	call->last = last;
}

void square_sum_chains(int v, number_t *last) {
	if (v == 0) {
		last->used = 0;
		return;
	}
	if (found) {
		return;
	}
	if (v == 1) {
		last->next_choices_n = last->first_choices_n;
	}
	last->used = 1;
	values[v-1] = last->value;
	if (add_call(0, last)) {
		if (v < n) {
			if (last->next_choices_n > 0) {
				int i;
				for (i = 0; i < last->first_choices_n; i++) {
					eval_number_next(last->choices[i]);
				}
				qsort(last->choices, (size_t)last->first_choices_n, sizeof(number_t *), compare_numbers_next);
				for (i = last->next_choices_n; i > 0 && add_call(v+1, last->choices[i-1]); i--);
			}
		}
		else {
			found = 1;
		}
	}
}

void eval_number_next(number_t *number) {
	int i;
	if (number->used) {
		return;
	}
	number->next_choices_n = 0;
	for (i = 0; i < number->first_choices_n; i++) {
		if (!number->choices[i]->used) {
			number->next_choices_n++;
		}
	}
}

int compare_numbers_next(const void *a, const void *b) {
	number_t *number_a = *(number_t * const *)a, *number_b = *(number_t * const *)b;
	if (number_a->used != number_b->used) {
		return number_a->used-number_b->used;
	}
	if (number_a->next_choices_n != number_b->next_choices_n) {
		return number_a->next_choices_n-number_b->next_choices_n;
	}
	return number_a->value-number_b->value;
}

void print_error(const char *error) {
	fprintf(stderr, "%s\n", error);
	fflush(stderr);
}
