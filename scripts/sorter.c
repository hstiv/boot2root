#include <dirent.h> 
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

typedef struct 	s_file
{
	int			f;
	char		*filename;
	struct s_file *next;
}				t_file;

t_file			*files;
int				max;

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void	add_file(char *filename, char *file_num)
{
	t_file *file;
	char	num[4];
	int		j;

	file = (t_file *)malloc(sizeof(t_file));
	file->filename = strdup(filename);
	file->next = NULL;
	bzero(num, 4);
	j = 0;
	for (int i = 0; file_num[i] != '\0'; i++)
	{
		if (isdigit(file_num[i]))
		{
			num[j] = file_num[i];
			j++;
		}
	}
	file->f = atoi(num);
	if (file->f > max)
		max = file->f;
	if (files == NULL)
		files = file;
	else
	{
		t_file *tmp = files;
		while (tmp && tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = file;
	}
}

int main(int argc, char ** argv)
{
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	max = 0;
	files = NULL;

	if (argc > 1)
	{
		FILE *fp = fopen(argv[1], "r");
		if (!fp)
		{
			fprintf(stderr, "Error opening file '%s'\n", argv[1]);
			return EXIT_FAILURE;
		}
		while (getline(&line_buf, &line_buf_size, fp) > 0)
		{
			if (strstr(line_buf, "//file"))
			{
				char **str = str_split(line_buf, ':');
				add_file(str[0], str[1]);
			}
		}
		t_file *temp;
		for(int i = 1; i <= max; i++)
		{
			temp = files;
			while (temp != NULL)
			{
				if (temp->f == i)
				{
					char *s = (char *)malloc(sizeof(char) * strlen("/bin/cat ") * strlen(temp->filename));
					s = strcat(s, "/bin/cat ft_fun/");
					system(strcat(s, temp->filename));
					break ;
				}
				temp = temp->next;
			}
		}
	}
	return (0);
}