CREATE TABLE Room (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    uid VARCHAR(30),
    offer TEXT,
    answer TEXT,
    ice_candidate_offer TEXT,
    ice_candidate_answer TEXT
)