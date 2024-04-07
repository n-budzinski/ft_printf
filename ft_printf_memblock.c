#include "ft_printf.h"

void	ft_lst_memblock_prepend(t_list **lst, void *mem, size_t size)
{
	t_list		*node;
	t_memblock	*memblock;

	memblock = malloc(sizeof(t_memblock));
	if (memblock == NULL)
		return ;
	memblock->data = mem;
	memblock->size = size;
	node = ft_lstnew(memblock);
	if (node)
		ft_lstadd_front(lst, node);
}

void	ft_lst_memblock_append(t_list **lst, void *mem, size_t size)
{
	t_list		*node;
	t_memblock	*memblock;

	memblock = malloc(sizeof(t_memblock));
	if (memblock == NULL)
		return ;
	memblock->data = mem;
	memblock->size = size;
	node = ft_lstnew(memblock);
	if (node)
		ft_lstadd_back(lst, node);
}

unsigned long long	ft_lst_memblock_getttlsize(t_list *lst)
{
	unsigned long long	size;

	size = 0;
	while (lst)
	{
		size += ((t_memblock *)(lst->content))->size;
		lst = lst->next;
	}
	return (size);
}

void	*ft_lst_memblock_join(t_list *lst)
{
	unsigned long long	size;
	unsigned long long	pos;
	void	*ptr;
	t_memblock	*tmp;

	size = ft_lst_memblock_getttlsize(lst);
	pos = 0;
	ptr = (void *)malloc(size);
	if (ptr == NULL)
		return (NULL);
	while (lst)
	{
		tmp = (t_memblock *)(lst->content);
		ft_memcpy(ptr + pos, tmp->data, tmp->size);
		pos += tmp->size;
		lst = lst->next;
	}
	return (ptr);
}

void	ft_lst_memblock_clear(void *memblock)
{
	if (memblock)
	{
		free(((t_memblock *)memblock)->data);
		free(memblock);
	}
}