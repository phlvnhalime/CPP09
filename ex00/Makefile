NAME = btc

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++11

SRCS = main.cpp BitcoinExchange.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm *.txt

fclean: clean
	rm -f $(NAME)
	rm *.txt

re: fclean all