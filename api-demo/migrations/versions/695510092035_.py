"""empty message

Revision ID: 695510092035
Revises: 
Create Date: 2019-03-21 16:23:31.261038

"""
from alembic import op
import sqlalchemy as sa

revision = '695510092035'
down_revision = None
branch_labels = None
depends_on = None


def upgrade():
    op.create_table('produtos',
    sa.Column('id', sa.Integer(), nullable=False),
    sa.Column('nome', sa.String(length=20), nullable=False),
    sa.Column('preco', sa.Float(asdecimal=True), nullable=False),
    sa.Column('origem', sa.String(length=20), nullable=False),
    sa.Column('quantidade', sa.Integer(), nullable=False),
    sa.Column('descricao', sa.String(length=255), nullable=True),
    sa.PrimaryKeyConstraint('id'),
    sa.UniqueConstraint('nome')
    )

def downgrade():
    op.drop_table('produtos')
