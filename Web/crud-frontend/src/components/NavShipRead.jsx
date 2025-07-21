import { useNavigate } from "react-router-dom";

export default function NavBar({ onOpen, onSearch }) {
  const navigate = useNavigate();

  const handleSearchChange = (event) => {
    onSearch(event.target.value); // Call the onSearch callback with the input value
  };

  return (
    <>
      <div className="navbar bg-base-100 p-4">
        <div className="navbar-start">
          <a className="btn btn-ghost text-xl">Shipments</a>
        </div>
        <div role="tablist" className="tabs tabs-box ">
          <a role="tab" className="tab tab-active">
            <button
              className="users-button cursor:pointer hover:bg-blue-700 rounded-full px-6"
              onClick={() => navigate("/")}
            >
              Shipments
            </button>
          </a>
          <a role="tab" className="tab">
            <button
              className="users-button cursor:pointer hover:bg-blue-700 rounded-full px-6"
              onClick={() => navigate("/readers")}
            >
              Readers
            </button>
          </a>
          <a role="tab" className="tab">
            <button
              className="users-button cursor:pointer hover:bg-blue-700 rounded-full px-6"
              onClick={() => navigate("/warehouse")}
            >
              Warehouse
            </button>
          </a>
          <a role="tab" className="tab">
            <button
              className="users-button cursor:pointer hover:bg-blue-700 rounded-full px-6"
              onClick={() => navigate("/tags")}
            >
              Tags
            </button>
          </a>
          <a role="tab" className="tab">
            <button
              className="users-button cursor:pointer hover:bg-blue-700 rounded-full px-6"
              onClick={() => navigate("/users")}
            >
              Users
            </button>
          </a>
        </div>
        <div className="navbar-center">
          <div className="form-control">
            <input
              type="text"
              placeholder="Search Shipments...."
              onChange={handleSearchChange}
              className="input input-bordered w-48 md:w-auto"
            />
          </div>
        </div>
        <div className="navbar-end">
          <a className="btn btn-primary" onClick={onOpen}>
            Add Shipments
          </a>
        </div>
      </div>
    </>
  );
}
