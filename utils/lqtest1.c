/* lqtest1.c --- 
 * 
 * 
 * Author: Jack F. Vasu
 * Created: Mon Nov  4 16:59:27 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

int main(){
	printf("making a LQ\n");

	lqueue_t* locked = lqopen();
	printf("putting an element in lockedQ\n");
	int x = 2;
	lqput(locked,&x);
	printf("putting another element in lockedQ\n");
	int y = 2;
	lqput(locked, &y);
	printf("getting element 1\n");
	int z = (int)lqget(locked);
	printf("first element is %d\n");
	int j = (int)lqget(locked);
	printf("second element is %d\n");
	lqclose(locked);
}
