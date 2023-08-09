#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

typedef struct Node {
  char path[255]; //DIR *d;??  //DIR* d??
  struct Node* next;
} Node;

void add_next(Node** root, char path_value[]) {
    Node* new_node = malloc(sizeof(Node));
    if(new_node == NULL) {
        exit(1);
    }
    new_node->next = NULL;
    strcpy(new_node->path, path_value);

    if (*root == NULL) {
        *root = new_node;
        return;
    }
    Node* curr = *root;
    while(curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = new_node;
}

void deallocate(Node** root) {
  Node* curr = *root;
  while(curr != NULL) {
    Node* aux = curr;
    curr = curr->next;
    free(aux);
  }
  *root = NULL;
}

int main() {
  printf("write a file name to search:");
  char searched[255];
  fgets(searched, sizeof searched, stdin);
  searched[strcspn(searched, "\n")] = 0;
  Node* root = malloc(sizeof(Node));
  if(root == NULL) {
    exit(2);
  }
  #ifdef _WIN32
    strcpy(root->path, "C:/");
  #else
    strcpy(root->path, "/");
  #endif
  root->next = NULL;
  //list all files in root directory:
  DIR *d;
  struct dirent *dir;
  d = opendir(&root->path); //assign directory
  if (d) {
        while ((dir = readdir(d)) != NULL) {
            char file_path [255];
            strcpy(file_path, root->path); //copy "previous" path
            strcat(file_path, dir->d_name);
            if(strcmp(searched, dir->d_name) == 0) {
                printf("%s\n", file_path);
                goto endprogram;
            } else if(dir->d_type == DT_DIR) {  //check if it is a directory
                strcat(file_path, "/"); // add slash to this directory
                add_next(&root, file_path);//add next directory name
            }
        }
        closedir(d);
    }

  //iteration
  // for(Node* curr = &startDirectory; curr != NULL; curr = curr->next) is smarter use it
  Node* curr = root->next;
  while (curr != NULL) {

    DIR *d;
    struct dirent *dir;
    d = opendir(&curr->path); //assign directory
    if (d) {
        while ((dir = readdir(d)) != NULL){

            char file_path [255];
            strcpy(file_path, curr->path); //copy "previous" path
            strcat(file_path, dir->d_name);

            if(strcmp(searched, dir->d_name) == 0) {
                printf("%s\n", file_path);
                goto endprogram;
            } else if(dir->d_type == DT_DIR) {  //check if it is a directory
                strcat(file_path, "/"); // add slash to this directory
                add_next(&root, file_path);//add next directory name
            }
        //add first elements to list
        }
    closedir(d);
    }

    curr = curr->next;
  }
endprogram:
  deallocate(&root);
  return 0;
}
