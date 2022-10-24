# Sending message

### sending to user

The way we use `user_id` is a bit tricky.

If you want to send a message to an user, you have
to be caution not to confuse `user_id` and `socket_id`.
Both are autoincrement id starting from  0.

For example, if you want to send a message to an user
you may use:

```cpp
send(find_user_by_nickname(value[1], all_users)->user_id + 1, "hello", 5, 0);
```
