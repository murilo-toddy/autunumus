from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/receive-number', methods=['POST'])
def receive_number():
    data = request.json
    if 'number' in data:
        received_number = data['number']
        response = {'message': f'Received number: {received_number}'}
        return jsonify(response), 200
    else:
        return jsonify({'error': 'Number not provided'}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
