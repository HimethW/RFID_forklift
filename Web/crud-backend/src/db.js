import pg from "pg"
import env from "dotenv"

env.config();

console.log({
  user: process.env.PG_USER,
  password: process.env.PG_PASSWORD,
  port: typeof process.env.PG_PORT,
});
const db = new pg.Client({
  user: process.env.PG_USER,
  host: process.env.PG_HOST,
  database: process.env.PG_DATABASE,
  password: process.env.PG_PASSWORD,
  port: parseInt(process.env.PG_PORT, 10), // <-- convert string to number here
});

//db.connect();

db.connect()
  .then(() => console.log("Connected to PostgreSQL"))
  .catch((err) => console.error("Connection error", err.stack));


db.on('error', (err) => {
    console.error('Unexpected error on idle client', err);
    process.exit(-1);
  });

export const query = (text, params) => db.query(text, params);