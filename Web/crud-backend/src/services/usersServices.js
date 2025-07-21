import { query } from "../db.js"

export const getClients = async() => {
    const {rows} = await query('SELECT * FROM Users');
    return rows;
}

export const createClient = async(clientData) => {
    const { user_id, name, role, password, last_login_time } = clientData;
    const { rows } = await query(
        `INSERT INTO Users (user_id, name, role, password, last_login_time) 
         VALUES ($1, $2, $3, $4, $5) RETURNING *`,
        [user_id, name, role, password, last_login_time]
      );
    
    return rows[0];
}


export const updateClient = async (clientId, clientData) => {
    const { user_id, name, role, password,last_login_time } = clientData;
  
    const { rows } = await query(
      `UPDATE Users SET user_id = $1, name = $2, role = $3, password = $4, last_login_time = $5 
       WHERE id = $6 RETURNING *`,
      [user_id, name, role, password, last_login_time, clientId]
    );
  
    return rows[0];
  };


export const deleteClient = async (clientId) => {
    const { rowCount } = await query(`DELETE FROM Users WHERE id = $1`, [clientId]);
    return rowCount > 0; // Returns true if a row was deleted, false otherwise
};

export const searchClients = async (searchTerm) => {
    const { rows } = await query(
      `SELECT * FROM Users WHERE user_id ILIKE $1 OR name ILIKE $1 OR role ILIKE $1`,
      [`%${searchTerm}%`]
    );
    return rows;
  };