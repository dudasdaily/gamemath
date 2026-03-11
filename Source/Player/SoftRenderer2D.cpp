
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// ���ڸ� �׸��� �Լ�
void SoftRenderer::DrawGizmo2D()
{
    auto& r = GetRenderer();
    const auto& g = Get2DGameEngine();

    // �׸��� ����
    LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

    // ���� ���� ���?
    Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
    Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

    // ���� �ϴܿ������� ���� �׸���
    int xGridCount = _ScreenSize.X / _Grid2DUnit;
    int yGridCount = _ScreenSize.Y / _Grid2DUnit;

    // �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���?
    Vector2 minPos = viewPos - extent;
    Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
    ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

    for (int ix = 0; ix < xGridCount; ++ix)
    {
        r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
    }

    for (int iy = 0; iy < yGridCount; ++iy)
    {
        r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
    }

    ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
    r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
    r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// ���� ������Ʈ ���?
static const std::string PlayerGo("Player");

// ���� �� �ε��� ����ϴ�? �Լ�
void SoftRenderer::LoadScene2D()
{
    // ���� �� �ε����� ����ϴ�? ���? �� �ֿ� ���۷���
    auto& g = Get2DGameEngine();

    // �÷��̾��� ������ ����
    constexpr float playerScale = 30.f;
    GameObject& goPlayer = g.CreateNewGameObject(PlayerGo);
    goPlayer.SetMesh(GameEngine::QuadMesh);
    goPlayer.GetTransform().SetScale(Vector2::One * playerScale);
    goPlayer.SetColor(LinearColor::Red);

    // 100���� ���? ���� ������Ʈ ������ ����
    char name[64];
    constexpr float squareScale = 20.f;
    std::mt19937 generator(0);
    std::uniform_real_distribution<float> dist(-1000.f, 1000.f);
    for (int i = 0; i < 100; ++i)
    {
        std::snprintf(name, sizeof(name), "GameObject%d", i);
        GameObject& newGo = g.CreateNewGameObject(name);
        newGo.GetTransform().SetPosition(Vector2(dist(generator), dist(generator)));
        newGo.GetTransform().SetScale(Vector2::One * squareScale);
        newGo.SetMesh(GameEngine::QuadMesh);
        newGo.SetColor(LinearColor::Blue);
    }
}

// ���� ������ ����ϴ�? �Լ�
void SoftRenderer::Update2D(float InDeltaSeconds)
{
    // ���� �������� ����ϴ�? ���? �� �ֿ� ���۷���
    auto& g = Get2DGameEngine();
    const InputManager& input = g.GetInputManager();

    // ���� ������ ���� ����
    static float moveSpeed = 200.f;
    static float rotateSpeed = 180.f;
    static float scaleMin = 15.f;
    static float scaleMax = 30.f;
    static float scaleSpeed = 180.f;
    static float minDistance = 1.f;
    static float lerpSpeed = 2.f;

    // �÷��̾ ���� �ֿ� ���۷���
    GameObject& goPlayer = g.GetGameObject(PlayerGo);
    TransformComponent& transform = goPlayer.GetTransform();

    // �Է¿� ���� �÷��̾� ��ġ�� ũ���� ����
    transform.AddPosition(Vector2(input.GetAxis(InputAxis::XAxis), input.GetAxis(InputAxis::YAxis)).GetNormalize() * moveSpeed * InDeltaSeconds);
    float newScale = Math::Clamp(transform.GetScale().X + scaleSpeed * input.GetAxis(InputAxis::ZAxis) * InDeltaSeconds, scaleMin, scaleMax);
    transform.SetScale(Vector2::One * newScale);
    transform.AddRotation(input.GetAxis(InputAxis::WAxis) * rotateSpeed * InDeltaSeconds);

    // �÷��̾ ����ٴϴ�? ī�޶��� Ʈ������
    TransformComponent& cameraTransform = g.GetMainCamera().GetTransform();
    Vector2 playerPos = transform.GetPosition();
    Vector2 cameraPos = cameraTransform.GetPosition();
    if ((playerPos - cameraPos).SizeSquared() < minDistance * minDistance)
    {
        cameraTransform.SetPosition(playerPos);
    }
    else
    {
        float ratio = Math::Clamp(lerpSpeed * InDeltaSeconds, 0.f, 1.f);
        Vector2 newCameraPos = cameraPos + (playerPos - cameraPos) * ratio;
        cameraTransform.SetPosition(newCameraPos);
    }
}

// ������ ������ ����ϴ�? �Լ�
void SoftRenderer::Render2D()
{
    // ������ �������� ����ϴ�? ���? �� �ֿ� ���۷���
    auto& r = GetRenderer();
    const auto& g = Get2DGameEngine();
    const auto& texture = g.GetTexture(GameEngine::BaseTexture);

    // ���? ���� �׸���
    DrawGizmo2D();

    // ������ ������ ���� ����
    size_t totalObjectCount = g.GetScene().size();
    Matrix3x3 viewMatrix = g.GetMainCamera().GetViewMatrix();

    // ���� �����ϴ� ���? ���� ������Ʈ�� ��ȸ
    for (auto it = g.SceneBegin(); it != g.SceneEnd(); ++it)
    {
        // ���� ������Ʈ�� ���۷����� ���?
        const GameObject& gameObject = *(*it);
        if (!gameObject.HasMesh() || !gameObject.IsVisible())
        {
            continue;
        }

        // �������� �ʿ��� ���� ������Ʈ�� �ֿ� ���۷����� ���?
        const Mesh& mesh = g.GetMesh(gameObject.GetMeshKey());
        const TransformComponent& transform = gameObject.GetTransform();
        Matrix3x3 finalMatrix = viewMatrix * transform.GetModelingMatrix();

        // ���� ������Ʈ�� ������ ����
        DrawMesh2D(mesh, finalMatrix, gameObject.GetColor());

        // �÷��̾��� ������ ȭ�鿡 ���?
        if (gameObject == PlayerGo)
        {
            r.PushStatisticText("Player Position : " + transform.GetPosition().ToString());
            r.PushStatisticText("Player Rotation : " + std::to_string(transform.GetRotation()) + " (deg)");
            r.PushStatisticText("Player Scale : " + std::to_string(transform.GetScale().X));
        }
    }
}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
    // �޽��� ������ �ľ��ϱ� ���� ���� ����
    size_t vertexCount = InMesh.GetVertices().size();
    size_t indexCount = InMesh.GetIndices().size();
    size_t triangleCount = indexCount / 3;

    // �޽� ������ �������� �����? ���� ���ۿ� �ε��� ���۷� ��ȯ
    std::vector<Vertex2D> vertices(vertexCount);
    std::vector<size_t> indice(InMesh.GetIndices());
    for (size_t vi = 0; vi < vertexCount; ++vi)
    {
        vertices[vi].Position = InMesh.GetVertices()[vi];
        if (InMesh.HasColor())
        {
            vertices[vi].Color = InMesh.GetColors()[vi];
        }

        if (InMesh.HasUV())
        {
            vertices[vi].UV = InMesh.GetUVs()[vi];
        }
    }

    // ���� ��ȯ ����
    VertexShader2D(vertices, InMatrix);

    // �׸����� ����
    FillMode fm = FillMode::None;
    if (InMesh.HasColor())
    {
        fm |= FillMode::Color;
    }
    if (InMesh.HasUV())
    {
        fm |= FillMode::Texture;
    }

    // �޽ø� �ﰢ������ �ɰ��� ���� �׸���
    for (int ti = 0; ti < triangleCount; ++ti)
    {
        int bi0 = ti * 3, bi1 = ti * 3 + 1, bi2 = ti * 3 + 2;
        std::vector<Vertex2D> tvs = { vertices[indice[bi0]] , vertices[indice[bi1]] , vertices[indice[bi2]] };
        DrawTriangle2D(tvs, InColor, fm);
    }
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
    // ������ �������� ����ϴ�? ���? �� �ֿ� ���۷���
    auto& r = GetRenderer();
    const GameEngine& g = Get2DGameEngine();
    const Texture& texture = g.GetTexture(GameEngine::BaseTexture);

    if (IsWireframeDrawing())
    {
        // ���̾����������� �޽ø� �׸���
        LinearColor finalColor = _WireframeColor;
        if (InColor != LinearColor::Error)
        {
            finalColor = InColor;
        }

        r.DrawLine(InVertices[0].Position, InVertices[1].Position, finalColor);
        r.DrawLine(InVertices[0].Position, InVertices[2].Position, finalColor);
        r.DrawLine(InVertices[1].Position, InVertices[2].Position, finalColor);
    }
    else
    {
        // �ﰢ�� ĥ�ϱ�
        // �ﰢ���� ���� ����
        Vector2 minPos(Math::Min3(InVertices[0].Position.X, InVertices[1].Position.X, InVertices[2].Position.X), Math::Min3(InVertices[0].Position.Y, InVertices[1].Position.Y, InVertices[2].Position.Y));
        Vector2 maxPos(Math::Max3(InVertices[0].Position.X, InVertices[1].Position.X, InVertices[2].Position.X), Math::Max3(InVertices[0].Position.Y, InVertices[1].Position.Y, InVertices[2].Position.Y));

        // �����߽���ǥ�� ���� ���� ���ͷ� ��ȯ
        Vector2 u = InVertices[1].Position - InVertices[0].Position;
        Vector2 v = InVertices[2].Position - InVertices[0].Position;

        // ���� �и� �� ( uv * uv - uu * vv )
        float udotv = u.Dot(v);
        float vdotv = v.Dot(v);
        float udotu = u.Dot(u);
        float denominator = udotv * udotv - vdotv * udotu;

        // ��ȭ �ﰢ���̸� �׸��� ����
        if (denominator == 0.f)
        {
            return;
        }

        float invDenominator = 1.f / denominator;

        ScreenPoint lowerLeftPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, minPos);
        ScreenPoint upperRightPoint = ScreenPoint::ToScreenCoordinate(_ScreenSize, maxPos);

        // �� ���� ȭ�� ���� ����� ���? Ŭ���� ó��
        lowerLeftPoint.X = Math::Max(0, lowerLeftPoint.X);
        lowerLeftPoint.Y = Math::Min(_ScreenSize.Y, lowerLeftPoint.Y);
        upperRightPoint.X = Math::Min(_ScreenSize.X, upperRightPoint.X);
        upperRightPoint.Y = Math::Max(0, upperRightPoint.Y);

        // �ﰢ�� ���� �� ���? ���� �����ϰ� ��ĥ
        for (int x = lowerLeftPoint.X; x <= upperRightPoint.X; ++x)
        {
            for (int y = upperRightPoint.Y; y <= lowerLeftPoint.Y; ++y)
            {
                ScreenPoint fragment = ScreenPoint(x, y);
                Vector2 pointToTest = fragment.ToCartesianCoordinate(_ScreenSize);
                Vector2 w = pointToTest - InVertices[0].Position;
                float wdotu = w.Dot(u);
                float wdotv = w.Dot(v);

                float s = (wdotv * udotv - wdotu * vdotv) * invDenominator;
                float t = (wdotu * udotv - wdotv * udotu) * invDenominator;
                float oneMinusST = 1.f - s - t;
                if (((s >= 0.f) && (s <= 1.f)) && ((t >= 0.f) && (t <= 1.f)) && ((oneMinusST >= 0.f) && (oneMinusST <= 1.f)))
                {
                    Vector2 targetUV = InVertices[0].UV * oneMinusST + InVertices[1].UV * s + InVertices[2].UV * t;
                    r.DrawPoint(fragment, FragmentShader2D(texture.GetSample(targetUV), LinearColor::White));
                }
            }
        }
    }
}
