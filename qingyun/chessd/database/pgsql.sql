BEGIN;

    CREATE TABLE users (
        user_id     serial PRIMARY KEY,
        user_name   varchar NOT NULL,
        user_type   varchar NOT NULL DEFAULT 'user',
        online_time integer NOT NULL DEFAULT 0,
        email       varchar,
        UNIQUE(user_name)
    );

    CREATE INDEX users_type_idx ON users ("user_type");

    CREATE TABLE banned_users (
        user_id     integer PRIMARY KEY REFERENCES users ON DELETE CASCADE,
        reason      varchar
    );

	CREATE TABLE banned_words (
		word		varchar NOT NULL,
		UNIQUE(word)
	);

    CREATE TABLE adjourned_games (
        game_id     serial PRIMARY KEY,
        category    varchar NOT NULL,
        time_stamp  integer NOT NULL,
        history     varchar NOT NULL
    );

    CREATE TABLE adjourned_game_players (
        game_id integer REFERENCES adjourned_games ON DELETE CASCADE,
        user_id integer REFERENCES users ON DELETE CASCADE,
        "role"  integer NOT NULL,
        time    integer NOT NULL,
        inc     integer NOT NULL
    );

    CREATE INDEX adjourned_games_game_id_idx ON adjourned_game_players (game_id);
    CREATE INDEX adjourned_games_user_id_idx ON adjourned_game_players (user_id);

    CREATE TABLE games (
        game_id     serial PRIMARY KEY,
        category    varchar NOT NULL,
        time_stamp  integer NOT NULL,
        history     varchar NOT NULL,
        result      varchar NOT NULL
    );

    CREATE TABLE game_players (
        game_id     integer REFERENCES games ON DELETE CASCADE,
        user_id     integer REFERENCES users ON DELETE CASCADE,
        result      integer NOT NULL,
        "role"      integer NOT NULL,
        time        integer NOT NULL,
        inc         integer NOT NULL
    );

    CREATE INDEX games_game_id_idx ON game_players (game_id);
    CREATE INDEX games_user_id_idx ON game_players (user_id);

    CREATE TABLE player_rating (
        user_id         integer REFERENCES users ON DELETE CASCADE,
        category        varchar NOT NULL,
        rating          integer NOT NULL,
        volatility      numeric NOT NULL,
        wins            integer NOT NULL,
        draws           integer NOT NULL,
        defeats         integer NOT NULL,
        max_rating      integer NOT NULL,
        max_timestamp   integer NOT NULL,
        last_game       integer NOT NULL,
        PRIMARY KEY(user_id, category)
    );

    CREATE INDEX player_rating_rating_idx ON player_rating (rating);
    CREATE INDEX player_rating_user_id_idx ON player_rating (user_id);

    CREATE TABLE annoucements (
        id          integer PRIMARY KEY,
        user_id     integer REFERENCES users ON DELETE CASCADE,
        time        integer NOT NULL,
        min_rating  integer NOT NULL,
        max_rating  integer NOT NULL,
        category    varchar NOT NULL
    );

    CREATE INDEX annoucements_user_id ON annoucements(user_id);
    CREATE INDEX annoucements_time ON annoucements(time);
    CREATE INDEX annoucements_min_rating ON annoucements(min_rating);
    CREATE INDEX annoucements_max_rating ON annoucements(max_rating);
    CREATE INDEX annoucements_category ON annoucements(category);

COMMIT;
