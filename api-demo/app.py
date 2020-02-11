from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate, MigrateCommand
from flask_script import Manager
from flask_restful import Resource, Api

app = Flask(__name__)
app.config.from_object('config')
api = Api(app)
db = SQLAlchemy(app)
migrate = Migrate(app, db)
manager = Manager(app)
manager.add_command('db', MigrateCommand)


class Produtos(db.Model):
    __tablename__ = 'produtos'
    id = db.Column(db.Integer, primary_key=True)
    nome = db.Column(db.String(20), unique=True, nullable=False)
    preco = db.Column(db.String(6, 2), nullable=False)
    origem = db.Column(db.String(20), nullable=False)
    quantidade = db.Column(db.Integer, nullable=False)
    descricao = db.Column(db.String(255), nullable=True)

    def __init__(self, nome, preco, origem, quantidade, descricao):
        self.nome = nome
        self.preco = preco
        self.origem = origem
        self.quantidade = quantidade
        self.descricao = descricao

    def __repr__(self):
        return '<Produto %d>' % self.id

    def to_json(self):
        json_produtos = {
            'id': self.id,
            'nome': self.nome,
            'preco': self.preco,
            'origem': self.origem,
            'quantidade': self.quantidade,
            'descricao': self.descricao
        }
        return json_produtos

    def from_json(json_produto):
        nome = json_produto.get('nome')
        preco = json_produto.get('preco')
        origem = json_produto.get('origem')
        quantidade = json_produto.get('quantidade')
        descricao = json_produto.get('descricao')
        return Produtos(nome=nome, preco=preco, origem=origem, quantidade=quantidade, descricao=descricao)


class Home(Resource):
    def get(self):
        query = Produtos.query.all()
        return jsonify({'Produtos': [Produtos.to_json() for Produtos in query]})

    def post(self):
        produto = Produtos.from_json(request.get_json())
        try:
            db.session.add(produto)
            db.session.commit()
            return jsonify({'Inserido': repr(produto)})
        except Exception as e:
            return jsonify({'error': e.args})


class Home_ID(Resource):
    def get(self, id):
        query = Produtos.query.get_or_404(id)
        return jsonify({'Produto': [query.to_json()]})

    def put(self, id):
        produto = Produtos.query.get_or_404(id)
        json_produto = request.get_json()
        atrib_Modificaveis = ['nome', 'preco', 'origem', 'quantidade', 'descricao']
        for atributos in atrib_Modificaveis:
            setattr(produto, atributos, json_produto.get(str(atributos)))
        try:
            db.session.add(produto)
            db.session.commit()
            return jsonify({'Modificado': repr(produto)})
        except Exception as e:
            return jsonify({'error': e.args})

    def delete(self, id):
        produto = Produtos.query.get_or_404(id)
        try:
            db.session.delete(produto)
            db.session.commit()
            return jsonify({'Excluído': repr(produto)})
        except Exception as e:
            return jsonify({'error': e.args})


api.add_resource(Home, '/produtos')
api.add_resource(Home_ID, '/produtos/<int:id>')
if __name__ == '__main__':
    manager.run()