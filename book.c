#include "book.h"

/*
 * @brief Allocate a new string, copy  the passed-in string over to it, and
 * return the new string. This function might be helpful when implementing later functions.
 *
 * Should fail (return NULL) if the input string is NULL or malloc fails
 *
 * @param const char *str, the string to copy
 * @return char*, a pointer to a new string
 */
char *dynamic_string_copy(const char *str) {
  if (str == NULL) {
    return NULL;
  }
  //UNUSED(str);
  char *newString = (char *) malloc(strlen(str) + 1 * sizeof(char));
  if(newString == NULL) {
    return NULL;
    //free(newString);
  }
  strcpy(newString, str);
  //free(newString);
  return (char*)newString;
}

/*
 * @brief Allocate a new AddressBook, and return a pointer to it. You should initialize
 * it to have a bucket array of size BUCKET_CAPACITY and a size of 0.
 *
 * Should fail (return NULL) if malloc fails
 *
 * @return AddressBook*, a pointer to the new address book. NULL if malloc fails
 */
AddressBook *create_book(void) {
  AddressBook *new = (AddressBook *) malloc(sizeof(AddressBook));
  if(new == NULL) {
    return NULL;
  }
  Node **newBucket = (Node **) calloc(BUCKET_CAPACITY, sizeof(Node *));
  if(newBucket == NULL) {
    free(new);
    return NULL;
  }
  new -> buckets = newBucket;
  new -> size = 0;
  
  return (AddressBook*) new;
  
  //return NULL;
}

/*
 * @brief Allocate and initialize a new Node with a copy of the given data that's
 * passed in as arguments, and return a pointer to it. Don't forget to set the
 * next member for this new Node to NULL.
 *
 * Should fail (return NULL) if any input string is NULL or the number of digits of
 * phone_number does not match PHONE_NUMBER_LEN or malloc fails
 *
 * @param const char *name, copy this string and use it as the new Node's name
 * @param const char *address, copy this string and use it as the new Node's address
 * @param long phone_number, use it as the new Node's phone_number
 * @return Node*, a pointer to the new node. NULL if malloc fails
 */
Node *create_node(const char *name, const char *address, long phone_number) {
  //UNUSED(name);
  //UNUSED(address);
  //UNUSED(phone_number);
  if (name == NULL || address == NULL) {
    return NULL;
  }
  long passedNum = phone_number;
  int count = 0;
  while (phone_number > 0) {
    phone_number /= 10;
    count++;
  }
  if (count != PHONE_NUMBER_LEN) {
      return NULL;
  }
  phone_number = passedNum;

  //method code
  Node *new = (Node *) malloc(sizeof(Node));
  char *newName = (char *) malloc(strlen(name) * sizeof(char) + 1);
  char *newAddress = (char *) malloc(strlen(address) * sizeof(char) + 1); 
  if(new == NULL) {
    return NULL;
  }
  if(newName == NULL) {
    free(new);
    return NULL;
  }
  if(newAddress == NULL) {
    free(new);
    free(newName);
    return NULL;
  }
  new -> name = newName;
  new -> address = newAddress;
  strcpy(new -> name, name);
  strcpy(new -> address, address);
  new -> phone_number = phone_number;
  new -> next = NULL;
  
  return new;
}

/*
 * @brief Create a new Node, put it into the AddressBook and increment the size of AddressBook. 
 *  
 * The bucket to put the node can be computed with hash(char *).
 * Add the node to the front of the bucket’s linked list.
 * If a node with the same name already exists, don’t make any changes to the table.
 *
 * Should fail if the given AddressBook is NULL, a node with the given name is found,
 * the given name or address is NULL, or phone_number does not have the right length. 
 * Should also fail if malloc fails at any point. Make sure to free things you have malloced along the way if you fail!
 *
 * @param AddressBook *book, the book to put the node
 * @param const char *name, the name of the node
 * @param const char *address, the address of the node
 * @param long phone_number, the phone_number of the node
 * @return int, returns SUCCESS if put is successful and FAILURE otherwise
 */
int put(AddressBook *book, const char *name, const char *address, long phone_number) {
  //UNUSED(book);
  //UNUSED(name);
  //UNUSED(address);
  //UNUSED(phone_number);
  if (book == NULL || name == NULL || address == NULL) {
    return FAILURE;
  }
  long passedNum = phone_number;
  int count = 0;
  while (phone_number > 0) {
    phone_number /= 10;
    count++;
  }
  if (count != PHONE_NUMBER_LEN) {
      return FAILURE;
  }
  phone_number = passedNum;

  //method code
  Node *newNode = create_node(name, address, phone_number);
  if (newNode == NULL) {
    return FAILURE;
  }
  Node *hasName = get(book, name);
  if (hasName != NULL) {
    free(newNode -> name);
    free(newNode -> address);
    free(newNode);
    return FAILURE;
  }
  int hashVal = hash(name);
  Node *head = book -> buckets[hashVal];
  newNode -> next = head;
  book -> buckets[hashVal] = newNode;
  book -> size += 1;
  return SUCCESS;
  
  //return FAILURE;
}

/*
 * @brief Remove the Node with the given name. Assign the predecessor Node's next to be the successor 
 * of the removed Node, and return the removed node’s name. If the node being removed is the first node 
 * in the linked list of a bucket, you should update the bucket as well.
 * 
 * Watch out for memory leaks in your implementation! Make sure you free anything you malloced appropriately!
 *
 * Should fail if the given book is NULL, or a node with the given name is not found
 *
 * @param AddressBook *book, the book to look into
 * @param const char *name, the name of the node to be removed
 * @return char*, a pointer to the name of the removed node or NULL if failure
 */
char *remove_node(AddressBook *book, const char *name) {
  //UNUSED(book);
  //UNUSED(name);
  if (book == NULL || name == NULL) {
    return NULL;
  }
  /*Node *hasName = get(book, name);
  if (hasName == NULL) {
    return NULL;
  }
  */
  int hashCode = hash(name);
  if (book -> buckets[hashCode] == NULL) {
    return NULL; 
  }
  Node *temp = book -> buckets[hashCode];
  Node *rem = book -> buckets[hashCode] -> next;
  char *retName;
  if (strcmp(temp -> name, name) == 0) {
    retName = temp -> name;
     book -> buckets[hashCode] = rem;
    //rem -> next = NULL;
    book -> size = book -> size - 1;
    free(temp -> address);
    free(temp);
    return retName; 
  } else {
    while (rem != NULL) {
      //rem = rem -> next;
      if (strcmp(rem -> name, name) == 0) {
        retName = rem -> name;
        temp -> next = rem -> next;
        rem -> next = NULL;
        book -> size = book -> size - 1;
        free(rem -> address);
        free(rem);
        return retName; 
      }
      rem = rem -> next;
      temp = temp -> next;
    }
  }
  //free(rem -> address);
  //free(rem);
  //return retName; 
  return NULL;
}

/*
 * @brief Search for a node in the book with the given name and return a pointer to the node.
 * Return NULL if it does not exist.
 *
 * Should fail (return NULL) if the given book is NULL, the given name is NULL, or a node 
 * with the given name is not found. 
 *
 * @param AddressBook *book, the book to search in
 * @param char *name, the name to search for in the book
 * @return Node*, a pointer to the found node
 */
Node *get(AddressBook *book, const char *name) {
  //UNUSED(book);
  //UNUSED(name);
  if (book == NULL || name == NULL) {
    return NULL;
  }
  /*for (int i = 0; i < book -> size; i++) {
    if (strcmp(book -> buckets[i]-> name, name) == 0) {
      return book -> buckets[i];
    }
  }*/
  int hashCode = hash(name);
  Node *head = book -> buckets[hashCode];
  while (head != NULL) {
    if (strcmp(head -> name, name) == 0) {
      return head;
    }
    head = head -> next;
  }
  
  return NULL;
  
}

/*
 * @brief Update the data of the Node with the given name. Return a pointer to the old name
 * 
 * Make sure you make copies of name and address when you update nodes!
 * 
 * Should fail (return NULL) if the given book is NULL，name or address is NULL, if the phone_number 
 * length isn't equal to PHONE_NUMBER_LEN or if a node with the given name is not found. Also fails 
 * if malloc fails.
 *
 * @param AddressBook *book, the book to look in
 * @param const char *name, the name to search for in the book
 * @param const char *address, the string to copy and update the node with
 * @param long phone_number, the int to copy and update the node with
 * @return char*, a pointer to the old name of the node. NULL if the method
 * fails
 */
char *update_node(AddressBook *book, const char *name, const char *address, long phone_number) {
  // UNUSED(book);
  // UNUSED(name);
  // UNUSED(address);
  // UNUSED(phone_number);
  if (book == NULL || name == NULL || address == NULL) {
    return NULL;
  }
  long passedNum = phone_number;
  int count = 0;
  while (phone_number > 0) {
    phone_number /= 10;
    count++;
  }
  if (count != PHONE_NUMBER_LEN) {
      return FAILURE;
  }
  phone_number = passedNum;
  Node *hasName = get(book, name);
  if (hasName == NULL) {
    return NULL;
  }

  //actual code
  Node *update = get(book, name);
  if(update == NULL) {
    return NULL;
  }
  /*char *retName = (char *) malloc(strlen(name) * sizeof(char) + 1);
  if (retName == NULL) {
    return NULL;
  }*/
  //strcpy(retName, update -> name);
  //char *temp = update -> name;

  //update -> name = retName;
  char *newAddress = (char *) realloc(update -> address, strlen(address) * sizeof(char) + 1);
  if (newAddress == NULL) {
    return NULL;
  }
  strcpy(newAddress, address);
  update -> address = newAddress;
  update -> phone_number = phone_number;
  // free(update -> name);
  // free(update -> address);
  // free(update);

  return update -> name;
}

/*
 * @brief Completely destroy the given bucket. Destroy all the nodes in the linked list
 * of the bucket. This means freeing any nodes linked to a specific bucket.
 * It will be in your best interest to reset the bucket's node pointer back to null.
 * If the index is out of bounds for BUCKET_CAPACITY, then simply return nothing.
 *
 * @param AddressBook *book, the book to look in
 * @param int bucket, the index of the bucket in the address book that will be destroyed
 */
void destroy_bucket(AddressBook *book, int bucket) {
  //UNUSED(book); 
  //UNUSED(bucket); 
  if (book == NULL || bucket >= BUCKET_CAPACITY) {
    return;
  }

  Node *curr = book -> buckets[bucket];
  Node *next;
  while (curr != NULL) {
    next = curr -> next;
    free(curr -> name);
    free(curr -> address);
    free(curr);
    curr = next;
  }
  book -> buckets[bucket] = NULL;
}

/*
 * @brief Completely destroy the given AddressBook. This means
 * freeing everything that the AddressBook references (all Nodes).
 * You can simply return nothing if the book is NULL.
 * 
 * @param AddressBook *book, the book to destroy
 */
void destroy_book(AddressBook *book) { 
  UNUSED(book); 
  if (book == NULL) {
    return;
  }
  for (int i = 0; i < BUCKET_CAPACITY; i++) {
    destroy_bucket(book, i);
  }
  free(book -> buckets);
  free(book);



}

  /*for (int i = 0; i < book -> size; i++) {
    rem -> next = 
    if (i == 0 && book -> buckets[i] -> name == name) {
      head -> next = 
      rem -> next = NULL;
      return book -> buckets[i] -> name;
    }
    if (book -> buckets[i] -> name == name) {
      head -> next = 
      rem -> next = NULL;
      return book -> buckets[i] -> name;
    }
    temp -> next = 
  }
  */
  
/*
 * @brief Computes the hash of a string
 * 
 * @param const char *name, the name to be hashed
 * @return int, the hash result
 */
int hash(const char *name) {
  int hash = 0;
  for (int i = 0; i < (int)strlen(name); i++) {
    hash += name[i];
  }
  return hash % BUCKET_CAPACITY;
}
