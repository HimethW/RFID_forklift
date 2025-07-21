import { useState, useEffect } from "react"

export default function ModalFormWarehouse({ isOpen , onClose, mode, OnSubmit,clientData}) {
    const [shelf_id, setShelf_ID] = useState('');
    const [shelf_loation, setshelf_location] = useState('');
    const [ warehouse_id, setWarehouseID] = useState('');
    const [warehouse_name, setWarehouse_Name] = useState('');



    const handleSubmit = async (e) => {
        e.preventDefault();
        try {
            const clientData = {shelf_id, shelf_loation, warehouse_id, warehouse_name}
            await OnSubmit(clientData)
            onClose();
        } catch (err) {
            console.error("Error adding shelf" , err);
        }
        
    }

    useEffect(() => {
        if (mode === 'edit' && clientData) {
            setShelf_ID(clientData.shelf_id);
            setshelf_location(clientData.shelf_loation);
            setWarehouseID(clientData.warehouse_id);
            setWarehouse_Name(clientData.warehouse_name);
        } else {
            // Reset fields when adding a new client
            setShelf_ID('');
            setshelf_location('');
            setWarehouseID('');
            setWarehouse_Name('');
        }
    }, [mode, clientData]);


    return (



        
        <>
            {/* You can open the modal using document.getElementById('ID').showModal() method */}
            <dialog id="my_modal_3" className="modal" open={isOpen}>
            <div className="modal-box">
                <h3 className="font-bold text-lg py-4">{mode === 'edit' ? 'Edit Shelves' : 'Shelve Details' }</h3>
                <form method="dialog" onSubmit={handleSubmit} >
                {/* if there is a button in form, it will close the modal */}

                <label className="input input-bordered my-4 flex items-center gap-2">
                    Shelf ID
                    <input type="text" className="grow" value={shelf_id} onChange={(e) => setShelf_ID(e.target.value)}/>
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Shelf Location
                    <input type="text" className="grow" value={shelf_loation} onChange={(e) => setshelf_location(e.target.value)} />
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Warehouse ID
                    <input type="text" className="grow" value={warehouse_id} onChange={(e) => setWarehouseID(e.target.value)}/>
                </label>
                <label className="input input-bordered my-4 flex items-center gap-2">
                    Warehouse Name
                    <input type="text" className="grow" value={warehouse_name} onChange={(e) => setWarehouse_Name(e.target.value)}/>
                </label>




                <button type="button" className="btn btn-sm btn-circle btn-ghost absolute right-2 top-2"  onClick={onClose}>✕</button>
                
                <button type="submit" className="btn btn-success"> {mode === 'edit' ? 'Save Changes' : 'Add Shelf' }</button>
                </form>
            </div>
            </dialog>
        </>
    )
}