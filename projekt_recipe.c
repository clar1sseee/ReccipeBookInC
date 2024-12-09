/*
 * projekt_recipe.c
 *
 *  Created on: Dec 27, 2022
 *      Author: Clarisse Tiongson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_INSTRUCTIONS_LENGTH 1000
#define MAX_INGREDIENTS_LENGTH 1000

typedef struct {
  char *name;
  char *ingredients;
  char *instructions;
} Recipe;


/**function select_recipe
 *
 * The recipe options are shown to the user, who can select one of them by a number.
 * When selected, the ingredients and instructions of the respective recipe are shown.
 */
void select_recipe(Recipe *all_recipes, int num_recipes) {
	printf("Select a recipe:\n");
	for (int i = 0; i < num_recipes; i++) {
		printf("%d: %s\n", i+1, all_recipes[i].name);
	}
		printf("Enter the number of the recipe you want to see: ");
		int recipe_selection;
		scanf("%d", &recipe_selection);
		Recipe *selected_recipe = &all_recipes[recipe_selection-1];
		printf("\n%s\n", selected_recipe->ingredients);
		printf("\n%s\n", selected_recipe->instructions);
}

/**function add_recipe
 *
 * This function gives the user an option to add a new recipe to the database.txt file.
 * When choosing this, the user can enter a recipe name, ingredients and instructions.
 * While putting in the ingredients and instructions, the user can write 'end' and the next task is being given.
 */
void add_recipe(Recipe *all_recipes, int *num_recipes) {
    printf("Enter the name of the new recipe: ");
    char name[MAX_LINE_LENGTH];
    scanf("%s", name);
    printf("Enter the ingredients (one per line, type 'end' to end): ");
    char new_ingredient[MAX_INGREDIENTS_LENGTH];
    char new_ingredients[MAX_INGREDIENTS_LENGTH] = "";
    while (1) {
    	scanf("%s%*c", new_ingredient);
    	if (strcmp(new_ingredient, "end") == 0) {
    		break;
    	}
    	strcat(new_ingredients, new_ingredient);
    	strcat(new_ingredients, "\n");
    }
    printf("Enter the instructions (one per line, type 'end' to end): ");
    char new_instruction[MAX_INSTRUCTIONS_LENGTH];
    char new_instructions[MAX_INSTRUCTIONS_LENGTH] = "";
    while (1) {
    	scanf("%s%*c", new_instruction);
    	if (strcmp(new_instruction, "end") == 0) {
    		break;
    	}
    	strcat(new_instructions, new_instruction);
    	strcat(new_instructions, " ");
    }

    Recipe new_recipe;
    new_recipe.name = name;
    new_recipe.ingredients = new_ingredients;
    new_recipe.instructions = new_instructions;
    all_recipes[*num_recipes] = new_recipe;
    *num_recipes += 1;

    FILE *f = fopen("database.txt", "a");
    fprintf(f, "\n\n-%s\nIngredients:\n%s\nInstructions:\n%s", new_recipe.name, new_recipe.ingredients, new_recipe.instructions);
    printf("File was successfully saved.\n");
    fclose(f);
}

/**function list_recipe_by_ingredients
 *
 * This function allows the user to search for certain ingredients.
 * The recipes including the given ingredients will be shown to the user.
 */
void list_recipes_by_ingredients(Recipe *all_recipes, int num_recipes) {
    printf("Enter the ingredients you have (separated by space): ");
    char ingredients[MAX_INGREDIENTS_LENGTH];
    scanf("%s", ingredients);

    char *ingredient = strtok(ingredients, " ");
    while (ingredient != NULL) {
    	for (int i = 0; i < num_recipes; i++){
    		Recipe recipe = all_recipes[i];
    	    if (strstr(recipe.ingredients, ingredient) != NULL) {
    	    	printf("Recipes that can be made with the specified ingredients:\n");
    	        printf("%s\n", recipe.name);
    	    }
    	}
    	ingredient = strtok(NULL, " ");
    }
}


/*function list_recipes_by_ingredients_and_amount
 *
 * This function allows the user to search for recipes that contain a specific ingredient and in a certain amount.
 * The user is prompted to enter an ingredient and an amount, and the program will then print the names of all
 * recipes that contain that ingredient in the specified amount.
 */
void list_recipes_by_ingredients_and_amount(Recipe *all_recipes, int num_recipes) {
    printf("Enter the ingredient to search for: ");
    char ingredient[MAX_LINE_LENGTH];
    scanf("%s", ingredient);
    printf("Enter the amount of the ingredient: ");
    char amount[MAX_LINE_LENGTH];
    scanf("%s", amount);
    printf("\nRecipes with %s %s:\n",amount,ingredient);
    for (int i = 0; i < num_recipes; i++) {
    	if (strstr(all_recipes[i].ingredients, ingredient) && strstr(all_recipes[i].ingredients, amount)) {
    		printf("%d: %s\n", i+1, all_recipes[i].name);
    	}
    }
}


/*function main
 * This function prints out the options the user can select.
 * The user can choose to view a recipe or search for ingredients in a recipe or add a recipe or exit.
 */
int main(int argc, char *argv[]) {
	FILE *f = fopen("database.txt", "r");
	if (f == NULL) {
		perror("Error opening file");
		return 1;
	}
	char *data = malloc(MAX_LINE_LENGTH);
	int max_line_length = MAX_LINE_LENGTH;
	max_line_length *= 2;
	int data_index = 0;
	char c;
	while ((c = fgetc(f)) != EOF) {
		data[data_index++] = c;
		if (data_index == MAX_LINE_LENGTH) {
			data = realloc(data, MAX_LINE_LENGTH * 2);
		}
		data[data_index] = '\0';
	}
	fclose(f);

	// Split the string into a list of recipe strings
	char **recipes = malloc(MAX_LINE_LENGTH);
	int num_recipes = 0;
	char *token = strtok(data, "-");
	while (token != NULL) {
		recipes[num_recipes++] = token;
		token = strtok(NULL, "-");
	 }



	Recipe *all_recipes = malloc(num_recipes * sizeof(Recipe));
	for (int i = 0; i < num_recipes; i++) {
		char *recipe_str = recipes[i];
		Recipe *recipe = &all_recipes[i];
		recipe->name = strtok(recipe_str, "\n");
	    recipe->ingredients = strtok(NULL,"");
	    recipe->instructions = strtok(NULL, "");
	}


	int option = 0;
	while (option != 5) {
		printf("===========================\n");
		printf("Select an option:\n");
		printf("1: View a recipe\n");
		printf("2: List recipes by ingredients\n");
		printf("3: List recipes by ingredients and amount\n");
		printf("4: Add a new recipe\n");
		printf("5: Exit\n");
		printf("Enter your selection: ");
		scanf("%d", &option);
		printf("===========================\n");

		switch(option) {
		case 1:{
			select_recipe(all_recipes, num_recipes);
			break;
		}
		case 2:{
		    list_recipes_by_ingredients(all_recipes, num_recipes);
		    break;
		}
		case 3:{
			list_recipes_by_ingredients_and_amount(all_recipes, num_recipes);
			break;
		}
		case 4:{
			add_recipe(all_recipes, &num_recipes);
			break;
		}case 5:{
		    printf("Exiting programm...");
		    break;
		}
		default:{
			printf("Invalid selection. Please enter a valid number.\n");
		}
		}
	}
	free(data);
	free(recipes);
	free(all_recipes);

	return 0;
}




