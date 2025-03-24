import React, { useState, useEffect } from 'react';
import { Button } from './button';
import { Card } from './card';
import { Separator } from './separator';
import { toast } from 'react-hot-toast';

interface Service {
  service_id: number;
  service_name: string;
}

interface Client {
  client_id: number;
  client_name: string;
  phone: string;
}

interface BookingFormProps {
  onClose: () => void;
  onSave: (bookingData: any) => void;
}

const AVAILABLE_COLORS = [
  { name: 'Розовый', hex: '#FF69B4' },
  { name: 'Красный', hex: '#FF0000' },
  { name: 'Зеленый', hex: '#00FF00' },
  { name: 'Голубой', hex: '#00BFFF' },
  { name: 'Оранжевый', hex: '#FFA500' },
];

export const BookingForm: React.FC<BookingFormProps> = ({ onClose, onSave }) => {
  const [activeTab, setActiveTab] = useState<'details' | 'client'>('details');
  const [services, setServices] = useState<Service[]>([]);
  const [clients, setClients] = useState<Client[]>([]);
  const [formData, setFormData] = useState({
    description: '',
    price: '',
    service_id: '',
    client_id: '',
    date: '',
    start_time: '',
    end_time: '',
    color: '#FF69B4',
  });

  useEffect(() => {
    // Загрузка списка услуг
    fetch('http://localhost:8180/services')
      .then(res => {
        console.log('Services response:', res);
        return res.json();
      })
      .then(data => {
        console.log('Services data:', data);
        setServices(data.services || []);
      })
      .catch(err => console.error('Error loading services:', err));

    // Загрузка списка клиентов
    fetch('http://localhost:8180/clients')
      .then(res => res.json())
      .then(data => setClients(data.clients || []))
      .catch(err => console.error('Error loading clients:', err));
  }, []);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    
    try {
      const startDate = new Date(formData.start_time);
      const endDate = new Date(formData.end_time);
      
      const bookingData = {
        description: formData.description || "Без описания",
        price: formData.price || "0",
        service_id: formData.service_id,
        client_id: formData.client_id || "0",
        date: startDate.toISOString().split('T')[0],
        start_time: startDate.toLocaleTimeString('en-US', { hour12: false }),
        end_time: endDate.toLocaleTimeString('en-US', { hour12: false }),
        color: formData.color
      };

      console.log('Sending booking data:', bookingData);

      const response = await fetch('http://localhost:8180/bookings', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Accept': 'application/json'
        },
        mode: 'cors',
        credentials: 'include',
        body: JSON.stringify(bookingData)
      });

      console.log('Response status:', response.status);
      const responseText = await response.text();
      console.log('Response text:', responseText);

      if (!response.ok) {
        throw new Error(responseText || 'Failed to create booking');
      }

      toast.success('Бронь успешно создана!');
      onSave(bookingData);
      onClose();
    } catch (error: any) {
      console.error('Error creating booking:', error);
      toast.error(`Ошибка создания брони: ${error.message || 'Неизвестная ошибка'}`);
    }
  };

  const handleInputChange = (e: React.ChangeEvent<HTMLInputElement | HTMLSelectElement>) => {
    const { name, value } = e.target;
    setFormData(prev => ({ ...prev, [name]: value }));
  };

  return (
    <div className="fixed inset-0 bg-transparent flex items-center justify-center z-50">
      <Card className="w-[500px] max-h-[90vh] overflow-y-auto bg-white shadow-lg rounded-lg">
        <div className="p-6">
          <div className="flex justify-between items-center mb-6">
            <div className="flex gap-4">
              <button
                className={`text-lg ${activeTab === 'details' ? 'text-blue-500 border-b-2 border-blue-500' : 'text-gray-500'}`}
                onClick={() => setActiveTab('details')}
              >
                Забронировать
              </button>
              <button
                className={`text-lg ${activeTab === 'client' ? 'text-blue-500 border-b-2 border-blue-500' : 'text-gray-500'}`}
                onClick={() => setActiveTab('client')}
              >
                Создать заметку
              </button>
            </div>
            <button onClick={onClose} className="text-gray-500 hover:text-gray-700">
              ×
            </button>
          </div>

          <div className="flex mb-4">
            <button
              className={`flex-1 py-2 ${activeTab === 'details' ? 'bg-gray-100' : ''}`}
              onClick={() => setActiveTab('details')}
            >
              Детали
            </button>
            <button
              className={`flex-1 py-2 ${activeTab === 'client' ? 'bg-gray-100' : ''}`}
              onClick={() => setActiveTab('client')}
            >
              Клиент
            </button>
          </div>

          <form onSubmit={handleSubmit}>
            <div className="space-y-4">
              <div>
                <label className="block text-sm font-medium text-gray-700">Клиент</label>
                <div className="mt-1 relative">
                  <select
                    name="client_id"
                    value={formData.client_id}
                    onChange={handleInputChange}
                    className="block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                  >
                    <option value="">Выберите клиента</option>
                    {clients.map(client => (
                      <option key={client.client_id} value={client.client_id}>
                        {client.client_name} ({client.phone})
                      </option>
                    ))}
                  </select>
                  <button
                    type="button"
                    className="absolute right-2 top-1/2 -translate-y-1/2 text-blue-500 hover:text-blue-700"
                  >
                    + Добавить
                  </button>
                </div>
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Услуга *</label>
                <select
                  name="service_id"
                  value={formData.service_id}
                  onChange={handleInputChange}
                  required
                  className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                >
                  <option value="">Выберите услугу</option>
                  {services.map(service => (
                    <option key={service.service_id} value={service.service_id}>
                      {service.service_name}
                    </option>
                  ))}
                </select>
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Комментарий</label>
                <input
                  type="text"
                  name="description"
                  value={formData.description}
                  onChange={handleInputChange}
                  className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Цена</label>
                <input
                  type="number"
                  name="price"
                  value={formData.price}
                  onChange={handleInputChange}
                  className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Время начала *</label>
                <input
                  type="datetime-local"
                  name="start_time"
                  value={formData.start_time}
                  onChange={handleInputChange}
                  required
                  className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Время окончания *</label>
                <input
                  type="datetime-local"
                  name="end_time"
                  value={formData.end_time}
                  onChange={handleInputChange}
                  required
                  className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-blue-500 focus:ring-blue-500"
                />
              </div>

              <div>
                <label className="block text-sm font-medium text-gray-700">Цвет</label>
                <div className="mt-2 flex gap-2">
                  {AVAILABLE_COLORS.map(color => (
                    <button
                      key={color.hex}
                      type="button"
                      onClick={() => setFormData(prev => ({ ...prev, color: color.hex }))}
                      className={`w-8 h-8 rounded-full ${
                        formData.color === color.hex ? 'ring-2 ring-offset-2 ring-blue-500' : ''
                      }`}
                      style={{ backgroundColor: color.hex }}
                      title={color.name}
                    />
                  ))}
                </div>
              </div>
            </div>

            <div className="mt-6 flex justify-end gap-4">
              <Button variant="outline" onClick={onClose}>
                Отменить
              </Button>
              <Button type="submit">Сохранить</Button>
            </div>
          </form>
        </div>
      </Card>
    </div>
  );
}; 